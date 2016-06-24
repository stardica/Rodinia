/*
 * =====================================================================================
 *
 *       Filename:  lud.cu
 *
 *    Description:  The main wrapper for the suite
 *
 *        Version:  1.0
 *        Created:  10/22/2009 08:40:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liang Wang (lw2aw), lw2aw@virginia.edu
 *        Company:  CS@UVa
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"
#include <sys/time.h>
#include <CL/cl.h>
#include <string.h>
#include <string>
#include <unistd.h>

#ifdef RD_WG_SIZE_0_0
#define BLOCK_SIZE RD_WG_SIZE_0_0
#elif defined(RD_WG_SIZE_0)
#define BLOCK_SIZE RD_WG_SIZE_0
#elif defined(RD_WG_SIZE)
#define BLOCK_SIZE RD_WG_SIZE
#else
#define BLOCK_SIZE 16
#endif

#define BEGIN_PARALLEL_SECTION 325
#define END_PARALLEL_SECTION 326


static int do_verify = 0;
static int use_gpu = 1;

static cl_context context;
static cl_command_queue cmd_queue;
static cl_device_type device_type;
static cl_device_id *device_list;
static cl_int num_devices;

static struct option long_options[] = {
      /* name, has_arg, flag, val */
      {"input", 1, NULL, 'i'},
      {"size", 1, NULL, 's'},
      {"verify", 0, NULL, 'v'},
      {0,0,0,0}
};


//Function prototypes
static int initialize(int);
static int shutdown(void);
double gettime(void);
cl_program CreateProgramFromBinary(cl_context context, cl_device_id device, const char* fileName);

#include "paths.h"


int main (int argc, char *argv[]){

	printf("WG size of kernel = %d X %d\n", BLOCK_SIZE, BLOCK_SIZE);
	int matrix_dim = 32; /* default matrix_dim */
	int opt, option_index=0;
	func_ret_t ret;
	const char *input_file = NULL;
	float *m, *mm;
	stopwatch sw;
	cl_int err = 0;
	
	while ((opt = getopt_long(argc, argv, "::vs:i:", long_options, &option_index)) != -1 )
	{

		switch(opt)
		{
			case 'i':
			input_file = optarg;
			break;

			case 'v':
			do_verify = 1;
			break;

			case 's':
			matrix_dim = atoi(optarg);
			printf("Generate input matrix internally, size =%d\n", matrix_dim);
			// fprintf(stderr, "Currently not supported, use -i instead\n");
			// fprintf(stderr, "Usage: %s [-v] [-s matrix_size|-i input_file]\n", argv[0]);
			// exit(EXIT_FAILURE);
			break;

			case '?':
			fprintf(stderr, "invalid option\n");
			break;

			case ':':
			fprintf(stderr, "missing argument\n");
			break;

			default:
			fprintf(stderr, "Usage: %s [-v] [-s matrix_size|-i input_file]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
  
	if((optind < argc) || (optind == 1))
	{
		fprintf(stderr, "Usage: %s [-v] [-s matrix_size|-i input_file]\n", argv[0]);
		exit(EXIT_FAILURE);
	}	

	if (input_file)
	{
		printf("Reading matrix from file %s\n", input_file);
		ret = create_matrix_from_file(&m, input_file, &matrix_dim);
		if (ret != RET_SUCCESS)
		{
			m = NULL;
			fprintf(stderr, "error create matrix from file %s\n", input_file);
			exit(EXIT_FAILURE);
		}
	} 
	else if (matrix_dim)
	{
	  printf("Creating matrix internally size=%d\n", matrix_dim);
	  ret = create_matrix(&m, matrix_dim);
	  	  if (ret != RET_SUCCESS)
	  	  {
	  		  m = NULL;
	  		  fprintf(stderr, "error create matrix internally size=%d\n", matrix_dim);
	  		  exit(EXIT_FAILURE);
	  	  }
	}
	else
	{
	  printf("No input file specified!\n");
	  exit(EXIT_FAILURE);
	}

	if (do_verify)
	{
		printf("Before LUD\n");
		matrix_duplicate(m, &mm, matrix_dim);
	}
	
	//print_matrix(m, matrix_dim);

	//int sourcesize = 1024*1024;
	//char * source = (char *)calloc(sourcesize, sizeof(char));

	//if(!source)
	//{
	//	printf("ERROR: calloc(%d) failed\n", sourcesize);
	//	return -1;
	//}

	char *kernel_lud_diag   = "lud_diagonal";
	char *kernel_lud_peri   = "lud_perimeter";
	char *kernel_lud_inter  = "lud_internal";


	// OpenCL initialization
	err = initialize(use_gpu);

	if(err)
	{
		printf("Failed to initialize.\n");
		return -1;
	}

	//star modified here.
	cl_program prog;

	prog = CreateProgramFromBinary(context, device_list[0], KERNEL);

	if(err != CL_SUCCESS)
	{
		printf("ERROR: clCreateProgramFromBinary() => %d\n", err);
		return -1;
	}

	char clOptions[110];
	//  sprintf(clOptions,"-I../../src"); 
	sprintf(clOptions," ");

	cl_kernel diagnal;
	cl_kernel perimeter;
	cl_kernel internal;

	diagnal   = clCreateKernel(prog, kernel_lud_diag, &err);
	perimeter = clCreateKernel(prog, kernel_lud_peri, &err);
	internal  = clCreateKernel(prog, kernel_lud_inter, &err);

	if(err != CL_SUCCESS)
	{
		printf("ERROR: clCreateKernel() 0 => %d\n", err); return -1;
	}

	clReleaseProgram(prog);
  
	//size_t local_work[3] = { 1, 1, 1 };
	//size_t global_work[3] = {1, 1, 1 }; 
  
	//star added this
	float * star_temp = (float*) malloc(sizeof(float)*matrix_dim*matrix_dim);
	cl_mem d_star_temp;
	d_star_temp = clCreateBuffer(context, CL_MEM_READ_WRITE, matrix_dim*matrix_dim * sizeof(float), NULL, &err, CL_TRUE);

	cl_mem d_m;
	d_m = clCreateBuffer(context, CL_MEM_READ_WRITE, matrix_dim*matrix_dim * sizeof(float), NULL, &err, CL_TRUE);
	
	if(err != CL_SUCCESS)
	{
		printf("ERROR: clCreateBuffer d_m (size:%d) => %d\n", matrix_dim*matrix_dim, err); return -1;
	}

	/* beginning of timing point */
	stopwatch_start(&sw);

	syscall(BEGIN_PARALLEL_SECTION);

	err = clEnqueueWriteBuffer(cmd_queue, d_m, 1, 0, matrix_dim*matrix_dim*sizeof(float), m, 0, 0, 0);
	if(err != CL_SUCCESS)
	{
		printf("ERROR: clEnqueueWriteBuffer d_m (size:%d) => %d\n", matrix_dim*matrix_dim, err); return -1;
	}

	int i=0;
	for (i=0; i < matrix_dim-BLOCK_SIZE; i += BLOCK_SIZE) 
	{

		//printf("In for loop\n");

		clSetKernelArg(diagnal, 0, sizeof(void *), (void*) &d_m);
		clSetKernelArg(diagnal, 1, sizeof(float) * BLOCK_SIZE * BLOCK_SIZE, (void*)NULL );
		clSetKernelArg(diagnal, 2, sizeof(cl_int), (void*) &matrix_dim);
		clSetKernelArg(diagnal, 3, sizeof(cl_int), (void*) &i);

		size_t global_work1[3]  = {BLOCK_SIZE, 1, 1};
		size_t local_work1[3]  = {BLOCK_SIZE, 1, 1};
	   
		err = clEnqueueNDRangeKernel(cmd_queue, diagnal, 2, NULL, global_work1, local_work1, 0, 0, 0);
		if(err != CL_SUCCESS)
		{
			printf("ERROR: diagnal clEnqueueNDRangeKernel()=>%d failed\n", err); return -1;
		}
		
		//star added this
		err = clEnqueueReadBuffer(cmd_queue, d_star_temp, 1, 0, matrix_dim*matrix_dim*sizeof(float), star_temp, 0, 0, 0);
		if(err != CL_SUCCESS)
		{
			printf("ERROR: clEnqueueReadBuffer()=>%d failed\n", err); return -1;
		}
		//printf("\n");	
		//print_matrix(star_temp, matrix_dim);
		
		//printf("ND Range 1.\n");
		//fflush(stdout);

		clSetKernelArg(perimeter, 0, sizeof(void *), (void*) &d_m);
		clSetKernelArg(perimeter, 1, sizeof(float) * BLOCK_SIZE * BLOCK_SIZE, (void*)NULL );
		clSetKernelArg(perimeter, 2, sizeof(float) * BLOCK_SIZE * BLOCK_SIZE, (void*)NULL );
		clSetKernelArg(perimeter, 3, sizeof(float) * BLOCK_SIZE * BLOCK_SIZE, (void*)NULL );
		clSetKernelArg(perimeter, 4, sizeof(cl_int), (void*) &matrix_dim);
		clSetKernelArg(perimeter, 5, sizeof(cl_int), (void*) &i);
	  
		size_t global_work2[3] = {BLOCK_SIZE * 2 * ((matrix_dim-i)/BLOCK_SIZE-1), 1, 1};
		size_t local_work2[3]  = {BLOCK_SIZE * 2, 1, 1};
	  
		err = clEnqueueNDRangeKernel(cmd_queue, perimeter, 2, NULL, global_work2, local_work2, 0, 0, 0);
		if(err != CL_SUCCESS)
		{
			printf("ERROR:  perimeter clEnqueueNDRangeKernel()=>%d failed\n", err); return -1;
		}
		//printf("ND Range 2.\n");
		//fflush(stdout);
	  
		clSetKernelArg(internal, 0, sizeof(void *), (void*) &d_m);
		clSetKernelArg(internal, 1, sizeof(float) * BLOCK_SIZE * BLOCK_SIZE, (void*)NULL );
		clSetKernelArg(internal, 2, sizeof(float) * BLOCK_SIZE * BLOCK_SIZE, (void*)NULL );
		clSetKernelArg(internal, 3, sizeof(cl_int), (void*) &matrix_dim);
		clSetKernelArg(internal, 4, sizeof(cl_int), (void*) &i);
      
		//size_t global_work3[2] = {BLOCK_SIZE * ((matrix_dim-i)/BLOCK_SIZE-1), BLOCK_SIZE * ((matrix_dim-i)/BLOCK_SIZE-1)};
		//size_t local_work3[2] = {BLOCK_SIZE, BLOCK_SIZE};
		size_t global_work3[3] = {BLOCK_SIZE * ((matrix_dim-i)/BLOCK_SIZE-1), BLOCK_SIZE * ((matrix_dim-i)/BLOCK_SIZE-1), 1};
		size_t local_work3[3] = {BLOCK_SIZE, BLOCK_SIZE, 1};

		err = clEnqueueNDRangeKernel(cmd_queue, internal, 2, NULL, global_work3, local_work3, 0, 0, 0);
		if(err != CL_SUCCESS)
		{
			printf("ERROR:  internal clEnqueueNDRangeKernel()=>%d failed\n", err); return -1;
		}
		//printf("ND Range 3.\n");
		//fflush(stdout);

	}

	printf("Out of for loop\n");

	clSetKernelArg(diagnal, 0, sizeof(void *), (void*) &d_m);
	clSetKernelArg(diagnal, 1, sizeof(float) * BLOCK_SIZE * BLOCK_SIZE, (void*)NULL );
	clSetKernelArg(diagnal, 2, sizeof(cl_int), (void*) &matrix_dim);
	clSetKernelArg(diagnal, 3, sizeof(cl_int), (void*) &i);
      
	size_t global_work1[3]  = {BLOCK_SIZE, 1, 1};
	size_t local_work1[3]  = {BLOCK_SIZE, 1, 1};
	err = clEnqueueNDRangeKernel(cmd_queue, diagnal, 2, NULL, global_work1, local_work1, 0, 0, 0);
	if(err != CL_SUCCESS)
	{
		printf("ERROR:  diagnal clEnqueueNDRangeKernel()=>%d failed\n", err); return -1;
	}
	

	//printf("ND Range 4.\n");
	//fflush(stdout);

	err = clEnqueueReadBuffer(cmd_queue, d_m, 1, 0, matrix_dim*matrix_dim*sizeof(float), m, 0, 0, 0);
	if(err != CL_SUCCESS)
	{
		printf("ERROR: clEnqueueReadBuffer  d_m (size:%d) => %d\n", matrix_dim*matrix_dim, err); return -1;
	}

	syscall(END_PARALLEL_SECTION);

	//printf("Read buffer.\n");
	//fflush(stdout);

	clFinish(cmd_queue);

	/* end of timing point */
	stopwatch_stop(&sw);
	printf("Time consumed(ms): %lf\n", 1000*get_interval_by_sec(&sw));

	clReleaseMemObject(d_m);

	//star added this here.
	//print_matrix(m, matrix_dim);

	if (do_verify)
	{
		printf("After LUD\n");
		printf("---Verified---\n");
		lud_verify(mm, m, matrix_dim); 
		free(mm);
	}

	free(m);
	
	if(shutdown())
		return -1;

	printf("---Done---\n");
	return 0;

}

static int initialize(int use_gpu){

	cl_int result;
	size_t size;

	// create OpenCL context
	cl_platform_id platform_id;
	if (clGetPlatformIDs(1, &platform_id, NULL) != CL_SUCCESS)
	{
		printf("ERROR: clGetPlatformIDs(1,*,0) failed\n");
		return 1;
	}

	cl_context_properties ctxprop[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0};

	device_type = use_gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU;
	context = clCreateContextFromType(ctxprop, device_type, NULL, NULL, NULL );

	if(!context)
	{
		printf("ERROR: clCreateContextFromType(%s) failed\n", use_gpu ? "GPU" : "CPU");
		return 1;
	}

	// get the list of GPUs
	result = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &size );

	num_devices = (int) (size / sizeof(cl_device_id));

	printf("num_devices = %d\n", num_devices);

	if( result != CL_SUCCESS || num_devices < 1 )
	{
		printf("ERROR: clGetContextInfo() failed\n");
		return 1;

	}

	device_list = new cl_device_id[num_devices];

	if( !device_list )
	{
		printf("ERROR: new cl_device_id[] failed\n");
		return 1;
	}

	result = clGetContextInfo(context, CL_CONTEXT_DEVICES, size, device_list, NULL );
	if( result != CL_SUCCESS )
	{
		printf("ERROR: clGetContextInfo() failed\n");
		return 1;
	}

	// create command queue for the first device
	cmd_queue = clCreateCommandQueue( context, device_list[0], 0, NULL );
	if( !cmd_queue )
	{
		printf("ERROR: clCreateCommandQueue() failed\n");
		return 1;
	}

	return 0;
}

static int shutdown(void){

	// release resources
	if( cmd_queue ) clReleaseCommandQueue( cmd_queue );
	if( context ) clReleaseContext( context );
	if( device_list ) delete device_list;

	// reset all variables
	cmd_queue = 0;
	context = 0;
	device_list = 0;
	num_devices = 0;
	device_type = 0;

	return 0;
}

double gettime(void) {

	struct timeval t;
	gettimeofday(&t,NULL);
	return t.tv_sec+t.tv_usec*1e-6;
}

cl_program CreateProgramFromBinary(cl_context context, cl_device_id device, const char* fileName)
{
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL)
    {
        return NULL;
    }

    // Determine the size of the binary
    size_t binarySize;
    fseek(fp, 0, SEEK_END);
    binarySize = ftell(fp);
    rewind(fp);

    unsigned char *programBinary;
    programBinary = (unsigned char *) malloc(sizeof(unsigned char[binarySize]));

    fread(programBinary, 1, binarySize, fp);
    fclose(fp);

    cl_int errNum = 0;
    cl_int binaryStatus;
    cl_program program;

    program = clCreateProgramWithBinary(context, 1, &device, &binarySize, (const unsigned char**)&programBinary, &binaryStatus,&errNum);

    if (errNum != CL_SUCCESS)
    {
        printf("Error loading program binary.\n");
        return NULL;
    }

    if (binaryStatus != CL_SUCCESS)
    {
        printf("Invalid binary for device,\n");
        return NULL;
    }


    errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    if (errNum != CL_SUCCESS)
    {
        // Determine the reason for the error
        char buildLog[16384];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);

        printf("CreateProgramFromBinary(): Error in kernel.\n");
        //printf("%s\n", buildLog);
        clReleaseProgram(program);
        return NULL;
    }

    free(programBinary);
    return program;
}
