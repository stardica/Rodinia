// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "backprop.h"
#include <CL/cl.h>
#include <unistd.h>

#if M2S_CGM_OCL_SIM == 0
	#include "cpucounters.h"
#endif

#include <time.h>
/*

#ifdef NV //NVIDIA
	#include <oclUtils.h>
#else 

#endif
*/

#include "paths.h"

#define BEGIN_PARALLEL_SECTION 325
#define END_PARALLEL_SECTION 326
#define CHECK_POINT 327

unsigned long long p_start, p_end, p_time, k_start, k_end, k_time;

clock_t start, end;
double cpu_time_used;

////////////////////////////////////////////////////////////////////////////////

// local variables
static cl_context	    context;
static cl_command_queue cmd_queue;
static cl_device_type   device_type;
static cl_device_id   * device_list;
static cl_int           num_devices;

static int initialize(int use_gpu)
{
	cl_int result;
	size_t size;

	// create OpenCL context
	cl_platform_id platform_id;
	if (clGetPlatformIDs(1, &platform_id, NULL) != CL_SUCCESS) { printf("ERROR: clGetPlatformIDs(1,*,0) failed\n"); return -1; }

	cl_context_properties ctxprop[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, 0};

	device_type = use_gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU;

	context = clCreateContextFromType( ctxprop, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL );
	if( !context ){ printf("ERROR: clCreateContextFromType(%s) failed\n", use_gpu ? "GPU" : "CPU"); return -1;}

	// get the list of GPUs
	result = clGetContextInfo( context, CL_CONTEXT_DEVICES, 0, NULL, &size );
	num_devices = (int) (size / sizeof(cl_device_id));
	printf("num_devices = %d\n", num_devices);
	
	if( result != CL_SUCCESS || num_devices < 1 ) { printf("ERROR: clGetContextInfo() failed\n"); return -1; }
	device_list = new cl_device_id[num_devices];
	//device_list = (cl_device_id *)malloc(sizeof(cl_device_id)*num_devices);
	if( !device_list ) { printf("ERROR: new cl_device_id[] failed\n"); return -1; }
	result = clGetContextInfo( context, CL_CONTEXT_DEVICES, size, device_list, NULL );
	if( result != CL_SUCCESS ) { printf("ERROR: clGetContextInfo() failed\n"); return -1; }

	// create command queue for the first device
	cmd_queue = clCreateCommandQueue( context, device_list[0], 0, NULL );
	if( !cmd_queue ) { printf("ERROR: clCreateCommandQueue() failed\n"); return -1; }
	return 0;
}

static int shutdown()
{
	// release resources
	if( cmd_queue ) clReleaseCommandQueue( cmd_queue );
	if( context ) clReleaseContext( context );
	if( device_list ) delete[] device_list;

	// reset all variables
	cmd_queue = 0;
	context = 0;
	device_list = 0;
	num_devices = 0;
	device_type = 0;

	return 0;
}

double gettime() {
  struct timeval t;
  gettimeofday(&t,NULL);
  return t.tv_sec+t.tv_usec*1e-6;
}

unsigned int num_threads = 0;
unsigned int num_blocks = 0;

#if M2S_CGM_OCL_SIM == 0
	//performance monitor
	PCM * m;

	CoreCounterState before_sstate, after_sstate;

	void intelPCM_init(){

		m = PCM::getInstance();

		m->resetPMU();

		PCM::ErrorCode returnResult = m->program();

		if (returnResult != PCM::Success)
		{
			std::cerr << "Intel's PCM couldn't start" << std::endl;
			std::cerr << "Error code: " << returnResult << std::endl;
			exit(1);
		}

		return;
	}
#endif

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) 
{

#if M2S_CGM_OCL_SIM == 0
	//intelPCM_init();
#endif


	setup(argc, argv);
}



int bpnn_train_kernel(BPNN *net, float *eo, float *eh)
{
	int in, hid, out;
	float out_err, hid_err;
  
	in = net->input_n;
	hid = net->hidden_n;
	out = net->output_n;   
   
	int sourcesize = 1024*1024;
	char * source = (char *)calloc(sourcesize, sizeof(char)); 
	if(!source) { printf("ERROR: calloc(%d) failed\n", sourcesize); return -1; }

	// read the kernel core source
	//char * kernel_bp1  = "bpnn_layerforward_ocl";
	//char * kernel_bp2  = "bpnn_adjust_weights_ocl";
	//char * tempchar = "./backprop_kernel.cl";
	//FILE * fp = fopen(tempchar, "rb");
	//if(!fp) { printf("ERROR: unable to open '%s'\n", tempchar); return -1; }
	//fread(source + strlen(source), sourcesize, 1, fp);
	//fclose(fp);
	
	int use_gpu = 1;
	if(initialize(use_gpu)) return -1;
	
	//star changes star here
	//get binanry file
	FILE *fp = fopen(KERNEL_PATH, "rb");
	if (fp == NULL)
	{
		printf("error opening kernel binary\n");
	}

	// Determine the size of the binary
	size_t binarySize;
	fseek(fp, 0, SEEK_END);
	binarySize = ftell(fp);
	rewind(fp);

	unsigned char *programBinary;
	programBinary = (unsigned char *) malloc(sizeof(unsigned char[binarySize]));

	size_t ret = fread(programBinary, 1, binarySize, fp);
	fclose(fp);

	cl_int binaryStatus;
	cl_int err = 0;

	cl_program prog = clCreateProgramWithBinary(context, 1, &device_list[0], &binarySize, (const unsigned char**)&programBinary, &binaryStatus, &err);

	if ((err != CL_SUCCESS) || (binaryStatus != CL_SUCCESS))
	{
		printf("error in clCreateProgramWithBinary\n");
	}



	// compile kernel
	//cl_int err = 0;
	//const char * slist[2] = { source, 0 };
	//cl_program prog = clCreateProgramWithSource(context, 1, slist, NULL, &err);
	//if(err != CL_SUCCESS) { printf("ERROR: clCreateProgramWithSource() => %d\n", err); return -1; }

	err = clBuildProgram(prog, 0, NULL, NULL, NULL, NULL);
	if(err != CL_SUCCESS) { printf("ERROR: clBuildProgram() => %d\n", err); return -1; }
    	
	//char * kernel_bp1  = "kernel_bp1";
	//char * kernel_bp2  = "kernel_bp2";



	char *kernel_bp1  = strdup("bpnn_layerforward_ocl");
	char *kernel_bp2  = strdup("bpnn_adjust_weights_ocl");

	cl_kernel kernel1;
	cl_kernel kernel2;
	fflush(stdout);
	kernel1 = clCreateKernel(prog, kernel_bp1, &err);
	kernel2 = clCreateKernel(prog, kernel_bp2, &err);
	if(err != CL_SUCCESS) { printf("ERROR: clCreateKernel() 0 => %d\n", err); return -1; }
	clReleaseProgram(prog);
	
	float *input_weights_one_dim;
    float *input_weights_prev_one_dim;
	float * partial_sum;
	float sum;
	float num_blocks = in / BLOCK_SIZE;
	
	input_weights_one_dim = (float *) malloc((in + 1)* (hid + 1) * sizeof(float));
	input_weights_prev_one_dim = (float *) malloc((in + 1)* (hid + 1) * sizeof(float));
	partial_sum = (float *) malloc(num_blocks * WIDTH * sizeof(float));
	
	// set global and local work items
	size_t global_work[3] = { BLOCK_SIZE, BLOCK_SIZE * num_blocks, 1 }; 
	size_t local_work[3] = { BLOCK_SIZE, BLOCK_SIZE, 1 };
	
	// this pre-processing stage is temporarily added to correct the bug of wrong memcopy using two-dimensional net->inputweights
	// todo: fix mem allocation
	int m = 0;
	for (int k = 0; k <= in; k++) {	
		for (int j = 0; j <= hid; j++) {
		input_weights_one_dim[m] = net->input_weights[k][j];
		input_weights_prev_one_dim[m] = net-> input_prev_weights[k][j];
	    m++;
		}
	}
	
	cl_mem input_hidden_ocl;
	cl_mem input_ocl;
	cl_mem output_hidden_ocl;
	cl_mem hidden_partial_sum;
	cl_mem hidden_delta_ocl;
	cl_mem input_prev_weights_ocl;

	#if M2S_CGM_OCL_SIM == 0
		p_start = RDTSC();
	#endif
	syscall(BEGIN_PARALLEL_SECTION);

	//start = clock();
  
	//star changed this whole block...

	/*printf("input_ocl addr 0x%08x net->input_units 0x%08x\n", &input_ocl, net->input_units);*/

#if M2S_CGM_OCL_SIM
	{
/*x*/   input_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (in + 1) * sizeof(float), net->input_units, &err, CL_TRUE);
/*x*/	input_hidden_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (in + 1) * (hid + 1) * sizeof(float), input_weights_one_dim, &err, CL_TRUE);
/*x*/	output_hidden_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (hid + 1) * sizeof(float), &output_hidden_ocl, &err, CL_TRUE);
/*x*/	hidden_partial_sum = clCreateBuffer(context, CL_MEM_READ_WRITE, num_blocks * WIDTH * sizeof(float), partial_sum, &err, CL_TRUE);
/*x*/	hidden_delta_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (hid + 1) * sizeof(float), net->hidden_delta, &err, CL_TRUE);
/*x*/	input_prev_weights_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (in + 1) * (hid + 1) * sizeof(float), input_weights_prev_one_dim, &err, CL_TRUE);

	}
#else
	{
		input_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (in + 1) * sizeof(float), NULL, &err);
		if(err != CL_SUCCESS) { printf("ERROR: clCreateBuffer input_ocl\n"); return -1;}

		input_hidden_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (in + 1) * (hid + 1) * sizeof(float), NULL, &err );
		if(err != CL_SUCCESS) { printf("ERROR: clCreateBuffer input_hidden_ocl\n"); return -1;}

		output_hidden_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (hid + 1) * sizeof(float), NULL, &err);
		if(err != CL_SUCCESS) { printf("ERROR: clCreateBuffer output_hidden_ocl\n"); return -1;}

		hidden_partial_sum = clCreateBuffer(context, CL_MEM_READ_WRITE, num_blocks * WIDTH * sizeof(float), NULL, &err);
		if(err != CL_SUCCESS) { printf("ERROR: clCreateBuffer hidden_partial_sum\n"); return -1;}

		hidden_delta_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (hid + 1) * sizeof(float), NULL, &err);
		if(err != CL_SUCCESS) { printf("ERROR: clCreateBuffer hidden_delta_ocl\n"); return -1;}

		input_prev_weights_ocl = clCreateBuffer(context, CL_MEM_READ_WRITE, (in + 1) * (hid + 1) * sizeof(float), NULL, &err );
		if(err != CL_SUCCESS) { printf("ERROR: clCreateBuffer input_prev_weights_ocl\n"); return -1;}
	}
#endif

	printf("Performing GPU computation\n");
	
	//write buffers
	err = clEnqueueWriteBuffer(cmd_queue, input_ocl, 1, 0, (in + 1) * sizeof(float), net->input_units, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer input_ocl\n"); return -1; }

	err = clEnqueueWriteBuffer(cmd_queue, input_hidden_ocl, 1, 0, (in + 1) * (hid + 1) * sizeof(float), input_weights_one_dim, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer input_hidden_ocl\n"); return -1; }
 
	clSetKernelArg(kernel1, 0, sizeof(void *), (void*) &input_ocl);
	clSetKernelArg(kernel1, 1, sizeof(void *), (void*) &output_hidden_ocl);
	clSetKernelArg(kernel1, 2, sizeof(void *), (void*) &input_hidden_ocl);
	clSetKernelArg(kernel1, 3, sizeof(void *), (void*) &hidden_partial_sum );
	clSetKernelArg(kernel1, 4, sizeof(float) *  HEIGHT, (void*)NULL );
	clSetKernelArg(kernel1, 5, sizeof(float ) *  HEIGHT * WIDTH, (void*)NULL );
	clSetKernelArg(kernel1, 6, sizeof(cl_int), (void*) &in);
	clSetKernelArg(kernel1, 7, sizeof(cl_int), (void*) &hid);
  
	#if M2S_CGM_OCL_SIM == 0
		k_start = RDTSC();
	#endif
	err = clEnqueueNDRangeKernel(cmd_queue, kernel1, 2, NULL, global_work, local_work, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: 1  clEnqueueNDRangeKernel()=>%d failed\n", err); return -1; }

	clFinish(cmd_queue);
	#if M2S_CGM_OCL_SIM == 0
		k_time += (RDTSC() - k_start);
	#endif
  
	err = clEnqueueReadBuffer(cmd_queue, hidden_partial_sum, 1, 0, num_blocks * WIDTH * sizeof(float), partial_sum, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: 1  clEnqueueReadBuffer: partial sum\n"); return -1; }	
  
	for (int j = 1; j <= hid; j++)
	{
		sum = 0.0;

		for (int k = 0; k < num_blocks; k++)
		{
			sum += partial_sum[k * hid + j-1] ;
		}

		sum += net->input_weights[0][j];
		net-> hidden_units[j] = float(1.0 / (1.0 + exp(-sum)));
	}

	bpnn_layerforward(net->hidden_units, net->output_units, net->hidden_weights, hid, out);
	bpnn_output_error(net->output_delta, net->target, net->output_units, out, &out_err);
	bpnn_hidden_error(net->hidden_delta, hid, net->output_delta, out, net->hidden_weights, net->hidden_units, &hid_err);  
	bpnn_adjust_weights(net->output_delta, out, net->hidden_units, hid, net->hidden_weights, net->hidden_prev_weights);


	err = clEnqueueWriteBuffer(cmd_queue, hidden_delta_ocl, 1, 0, (hid + 1) * sizeof(float), net->hidden_delta, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer hidden_delta_ocl\n"); return -1; }

	err = clEnqueueWriteBuffer(cmd_queue, input_prev_weights_ocl, 1, 0, (in + 1) * (hid + 1) * sizeof(float), input_weights_prev_one_dim, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer input_prev_weights_ocl\n"); return -1; }

	err = clEnqueueWriteBuffer(cmd_queue, input_hidden_ocl, 1, 0, (in + 1) * (hid + 1) * sizeof(float), input_weights_one_dim, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer input_hidden_ocl\n"); return -1; }
  
	clSetKernelArg(kernel2, 0, sizeof(void *), (void*) &hidden_delta_ocl);
	clSetKernelArg(kernel2, 1, sizeof(cl_int), (void*) &hid);
	clSetKernelArg(kernel2, 2, sizeof(void *), (void*) &input_ocl);
	clSetKernelArg(kernel2, 3, sizeof(cl_int), (void*) &in);
	clSetKernelArg(kernel2, 4, sizeof(void *), (void*) &input_hidden_ocl);
	clSetKernelArg(kernel2, 5, sizeof(void *), (void*) &input_prev_weights_ocl );
  
	#if M2S_CGM_OCL_SIM == 0
		k_start = RDTSC();
	#endif

	err = clEnqueueNDRangeKernel(cmd_queue, kernel2, 2, NULL, global_work, local_work, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: 1  clEnqueueNDRangeKernel()=>%d failed\n", err); return -1; }

	clFinish(cmd_queue);
	#if M2S_CGM_OCL_SIM == 0
		k_time += (RDTSC() - k_start);
	#endif
  
	err = clEnqueueReadBuffer(cmd_queue, input_ocl, 1, 0, (in + 1) * sizeof(float), net->input_units, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: 1  clEnqueueReadBuffer: input_ocl\n"); return -1; }	

	err = clEnqueueReadBuffer(cmd_queue, input_hidden_ocl, 1, 0, (in + 1) * (hid + 1) * sizeof(float), input_weights_one_dim, 0, 0, 0);
	if(err != CL_SUCCESS) { printf("ERROR: 1  clEnqueueReadBuffer: input_hidden_ocl\n"); return -1; }



	//end = clock();

	//printf("p_sections start %llu\n", start);

	//printf("p_sections end %llu\n", end);

	//unsigned long long p_time = p_end - p_start;
	//printf("p_sections cycles %llu\n", p_time);

	//cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	//printf("tick %llu\n", CLOCKS_PER_SEC);

	//printf("cpu_time_used %llu\n", end - start);

	syscall(END_PARALLEL_SECTION);

	#if M2S_CGM_OCL_SIM == 0
		p_time = (RDTSC() - p_start);
	#endif


	printf("Parallel Section Cycles %llu Kernel Cycles %llu\n", p_time, k_time);


	clReleaseMemObject(input_ocl);
	clReleaseMemObject(output_hidden_ocl);
	clReleaseMemObject(input_hidden_ocl);
	clReleaseMemObject(hidden_partial_sum);
	clReleaseMemObject(input_prev_weights_ocl);

	free(input_weights_prev_one_dim);
	free(partial_sum);
	free(input_weights_one_dim);

}

