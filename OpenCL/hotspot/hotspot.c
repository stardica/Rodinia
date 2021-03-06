#include "hotspot.h"
#include "paths.h"
#include <unistd.h>

#if M2S_CGM_OCL_SIM == 0
	#include "cpucounters.h"
#endif

#define BEGIN_PARALLEL_SECTION 325
#define END_PARALLEL_SECTION 326
#define CHECK_POINT 327

unsigned long long p_start, p_end, p_time, k_start, k_end, k_time;

void writeoutput(float *vect, int grid_rows, int grid_cols, char *file) {

	int i,j, index=0;
	FILE *fp;
	char str[STR_SIZE];

	if( (fp = fopen(file, "w" )) == 0 )
          printf( "The file was not opened\n" );


	for (i=0; i < grid_rows; i++) 
	 for (j=0; j < grid_cols; j++)
	 {
		 sprintf(str, "%d\t%g\n", index, vect[i*grid_cols+j]);
		 fputs(str,fp);
		 index++;
	 }
		
      fclose(fp);	
}


void readinput(float *vect, int grid_rows, int grid_cols, char *file) {

  	int i,j;
	FILE *fp;
	char str[STR_SIZE];
	float val;

	if( (fp  = fopen(file, "r" )) ==0 )
            fatal_hotspot( "The file was not opened" );


	for (i=0; i <= grid_rows-1; i++) 
	 for (j=0; j <= grid_cols-1; j++)
	 {
		if (fgets(str, STR_SIZE, fp) == NULL) fatal_hotspot("Error reading file\n");
		if (feof(fp))
			fatal_hotspot("not enough lines in file");
		//if ((sscanf(str, "%d%f", &index, &val) != 2) || (index != ((i-1)*(grid_cols-2)+j-1)))
		if ((sscanf(str, "%f", &val) != 1))
			fatal_hotspot("invalid file format");
		vect[i*grid_cols+j] = val;
	}

	fclose(fp);	

}


/*
   compute N time steps
*/

int compute_tran_temp(cl_mem MatrixPower, cl_mem MatrixTemp[2], int col, int row, \
		int total_iterations, int num_iterations, int blockCols, int blockRows, int borderCols, int borderRows,
		float *TempCPU, float *PowerCPU) 
{ 
	
	float grid_height = chip_height / row;
	float grid_width = chip_width / col;

	float Cap = FACTOR_CHIP * SPEC_HEAT_SI * t_chip * grid_width * grid_height;
	float Rx = grid_width / (2.0 * K_SI * t_chip * grid_height);
	float Ry = grid_height / (2.0 * K_SI * t_chip * grid_width);
	float Rz = t_chip / (K_SI * grid_height * grid_width);

	float max_slope = MAX_PD / (FACTOR_CHIP * t_chip * SPEC_HEAT_SI);
	float step = PRECISION / max_slope;
	int t;

	int src = 0, dst = 1;
	
	cl_int error;
	
	// Determine GPU work group grid
	size_t global_work_size[2];
	global_work_size[0] = BLOCK_SIZE * blockCols;
	global_work_size[1] = BLOCK_SIZE * blockRows;
	size_t local_work_size[2];
	local_work_size[0] = BLOCK_SIZE;
	local_work_size[1] = BLOCK_SIZE;
	
	
	long long start_time = get_time();

	for (t = 0; t < total_iterations; t ++)
	{
		printf("Run kernel %d of %d\n", (t + 1), total_iterations);

		// Specify kernel arguments
		int iter = MIN(num_iterations, total_iterations - t);
		clSetKernelArg(kernel, 0, sizeof(int), (void *) &iter);
		clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *) &MatrixPower);
		clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *) &MatrixTemp[src]);
		clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *) &MatrixTemp[dst]);
		clSetKernelArg(kernel, 4, sizeof(int), (void *) &col);
		clSetKernelArg(kernel, 5, sizeof(int), (void *) &row);
		clSetKernelArg(kernel, 6, sizeof(int), (void *) &borderCols);
		clSetKernelArg(kernel, 7, sizeof(int), (void *) &borderRows);
		clSetKernelArg(kernel, 8, sizeof(float), (void *) &Cap);
		clSetKernelArg(kernel, 9, sizeof(float), (void *) &Rx);
		clSetKernelArg(kernel, 10, sizeof(float), (void *) &Ry);
		clSetKernelArg(kernel, 11, sizeof(float), (void *) &Rz);
		clSetKernelArg(kernel, 12, sizeof(float), (void *) &step);
		
		// Launch kernel
		#if M2S_CGM_OCL_SIM == 0
			k_start = RDTSC();
		#endif
		error = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
		
		// Flush the queue
		error = clFlush(command_queue);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

		error = clFinish(command_queue);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

		#if M2S_CGM_OCL_SIM == 0
			k_time += (RDTSC() - k_start);
		#endif
		

		// Swap input and output GPU matrices
		src = 1 - src;
		dst = 1 - dst;

		// Wait for all operations to finish

	}

	printf("Done running kernels\n");
	
	long long end_time = get_time();
	long long total_time = (end_time - start_time);	
	printf("\nKernel time: %.3f seconds\n", ((float) total_time) / (1000*1000));
	
	return src;
}

void usage(int argc, char **argv) {
	fprintf(stderr, "Usage: %s <grid_rows/grid_cols> <pyramid_height> <sim_time> <temp_file> <power_file> <output_file>\n", argv[0]);
	fprintf(stderr, "\t<grid_rows/grid_cols>  - number of rows/cols in the grid (positive integer)\n");
	fprintf(stderr, "\t<pyramid_height> - pyramid heigh(positive integer)\n");
	fprintf(stderr, "\t<sim_time>   - number of iterations\n");
	fprintf(stderr, "\t<temp_file>  - name of the file containing the initial temperature values of each cell\n");
	fprintf(stderr, "\t<power_file> - name of the file containing the dissipated power values of each cell\n");
	fprintf(stderr, "\t<output_file> - name of the output file\n");
	exit(1);
}

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

int main(int argc, char** argv) {

	#if M2S_CGM_OCL_SIM == 0
		intelPCM_init();
	#endif

  printf("WG size of kernel = %d X %d\n", BLOCK_SIZE, BLOCK_SIZE);


    int size;
    int grid_rows,grid_cols = 0;
    float *FilesavingTemp,*FilesavingPower, *swap; //,*MatrixOut;
    char *tfile, *pfile, *ofile;
    
    int total_iterations = 60;
    int pyramid_height = 1; // number of iterations
	
	if (argc < 7)
		usage(argc, argv);
	if((grid_rows = atoi(argv[1]))<=0||
	   (grid_cols = atoi(argv[1]))<=0||
       (pyramid_height = atoi(argv[2]))<=0||
       (total_iterations = atoi(argv[3]))<=0)
		usage(argc, argv);
		
    tfile=argv[4];
    pfile=argv[5];
    ofile=argv[6];
	
    size=grid_rows*grid_cols;

    // --------------- pyramid parameters --------------- 
    int borderCols = (pyramid_height)*EXPAND_RATE/2;
    int borderRows = (pyramid_height)*EXPAND_RATE/2;
    int smallBlockCol = BLOCK_SIZE-(pyramid_height)*EXPAND_RATE;
    int smallBlockRow = BLOCK_SIZE-(pyramid_height)*EXPAND_RATE;
    int blockCols = grid_cols/smallBlockCol+((grid_cols%smallBlockCol==0)?0:1);
    int blockRows = grid_rows/smallBlockRow+((grid_rows%smallBlockRow==0)?0:1);

    FilesavingTemp = (float *) malloc(size*sizeof(float));
    FilesavingPower = (float *) malloc(size*sizeof(float));

    //star added this for the OCL changes...
    swap = (float *) malloc(size*sizeof(float));


    // MatrixOut = (float *) calloc (size, sizeof(float));

    if( !FilesavingPower || !FilesavingTemp) // || !MatrixOut)
        fatal_hotspot("unable to allocate memory");
	
	// Read input data from disk
    printf("reading input temp file\n");
    readinput(FilesavingTemp, grid_rows, grid_cols, tfile);
    printf("reading input power file\n");
    readinput(FilesavingPower, grid_rows, grid_cols, pfile);
    printf("finished reading in files\n");
	

    syscall(CHECK_POINT);

    cl_int error;
	cl_uint num_platforms;

	// Get the number of platforms
	error = clGetPlatformIDs(0, NULL, &num_platforms);
    if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

	// Get the list of platforms
	cl_platform_id* platforms = (cl_platform_id *) malloc(sizeof(cl_platform_id) * num_platforms);
	error = clGetPlatformIDs(num_platforms, platforms, NULL);
    if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

	// Print the chosen platform (if there are multiple platforms, choose the first one)
	cl_platform_id platform = platforms[0];
	char pbuf[100];
	error = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, sizeof(pbuf), pbuf, NULL);
	if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	printf("Platform: %s\n", pbuf);

	// Create a GPU context
	cl_context_properties context_properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties) platform, 0};
    context = clCreateContextFromType(context_properties, CL_DEVICE_TYPE_GPU, NULL, NULL, &error);
    if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

	// Get and print the chosen device (if there are multiple devices, choose the first one)
	size_t devices_size;
	error = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &devices_size);
	if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	cl_device_id *devices = (cl_device_id *) malloc(devices_size);
	error = clGetContextInfo(context, CL_CONTEXT_DEVICES, devices_size, devices, NULL);
	if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	device = devices[0];
	error = clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(pbuf), pbuf, NULL);
	if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	printf("Device: %s\n", pbuf);

	// Create a command queue
	command_queue = clCreateCommandQueue(context, device, 0, &error);
    if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

    //star chagnes start here
    	// Load kernel source from file
	//const char *source = load_kernel_source("/home/stardica/Desktop/m2sRodiniaBenchmarks/Rodinia/OpenCL/HotSpot/Release/hotspot_kernel.cl");
	//size_t sourceSize = strlen(source);

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

	        fread(programBinary, 1, binarySize, fp);
	        fclose(fp);

	        cl_int binaryStatus;


	    //oclHandles.program = clCreateProgramWithSource(oclHandles.context, 1, &source, sourceSize, &resultCL);
	    cl_program program = clCreateProgramWithBinary(context, 1, &device, &binarySize, (const unsigned char**)&programBinary, &binaryStatus, &error);

	    if (error != CL_SUCCESS)
	    {
	    	printf("error in clCreateProgramWithBinary\n");
	    }

	    if (binaryStatus != CL_SUCCESS)
	    {
	        printf("error in clCreateProgramWithBinary\n");
	    }
	
	// Compile the kernel
    	//cl_program program = clCreateProgramWithSource(context, 1, &source, &sourceSize, &error);
    	//if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	
	char clOptions[110];
	//  sprintf(clOptions,"-I../../src"); 
	sprintf(clOptions," ");
#ifdef BLOCK_SIZE
	sprintf(clOptions + strlen(clOptions), " -DBLOCK_SIZE=%d", BLOCK_SIZE);
#endif

    // Create an executable from the kernel
	error = clBuildProgram(program, 1, &device, clOptions, NULL, NULL);
	// Show compiler warnings/errors
	static char log[65536]; 
	memset(log, 0, sizeof(log));
	
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(log)-1, log, NULL);
	if (strstr(log,"warning:") || strstr(log, "error:")) printf("<<<<\n%s\n>>>>\n", log);
    	if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
    
	kernel = clCreateKernel(program, "hotspot", &error);
    	if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	
		
    //printf("program and kernel created\n");
	//getchar();

	long long start_time = get_time();
	
	// Create two temperature matrices and copy the temperature input data
	cl_mem MatrixTemp[2];
	cl_mem MatrixPower = NULL;


	#if M2S_CGM_OCL_SIM == 0
		p_start = RDTSC();
	#endif

	syscall(BEGIN_PARALLEL_SECTION);

	#if M2S_CGM_OCL_SIM
	{
		// Create input memory buffers on device
		MatrixTemp[0] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float) * size, FilesavingTemp, &error, CL_TRUE);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	
		//MatrixTemp[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, sizeof(float) * size, NULL, &error);
		/*x*/MatrixTemp[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float) * size, swap, &error, CL_TRUE);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

		// Copy the power input data
		MatrixPower = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * size, FilesavingPower, &error, CL_TRUE);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	}
#else
	{
		// Create input memory buffers on device
		MatrixTemp[0] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(float) * size, FilesavingTemp, &error);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

		MatrixTemp[1] = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, sizeof(float) * size, NULL, &error);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

		// Copy the power input data
		MatrixPower = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * size, FilesavingPower, &error);
		if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	}
#endif
	
	//write buffers
	error = clEnqueueWriteBuffer(command_queue, MatrixTemp[0], 1, 0, sizeof(float) * size, FilesavingTemp, 0, 0, 0);
	if(error != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer input_ocl\n"); return -1; }

	//write buffers
	error = clEnqueueWriteBuffer(command_queue, MatrixTemp[1], 1, 0, sizeof(float) * size, swap, 0, 0, 0);
	if(error != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer input_ocl\n"); return -1; }

	//write buffers
	error = clEnqueueWriteBuffer(command_queue, MatrixPower, 1, 0, sizeof(float) * size, FilesavingPower, 0, 0, 0);
	if(error != CL_SUCCESS) { printf("ERROR: clEnqueueWriteBuffer input_ocl\n"); return -1; }


	// Perform the computation
	int ret = compute_tran_temp(MatrixPower, MatrixTemp, grid_cols, grid_rows, total_iterations, pyramid_height,
				    blockCols, blockRows, borderCols, borderRows, FilesavingTemp, FilesavingPower);
	

	//printf("ret %d mt0 0x%08x mt1 0x%08x swap 0x%08x ftemp 0x%08x\n",
	//		ret, (unsigned int)&MatrixTemp[0], (unsigned int)&MatrixTemp[1], (unsigned int)swap, (unsigned int)FilesavingTemp);

	// Copy final temperature data back
	if(ret == 0)
	{
		error = clEnqueueReadBuffer(command_queue, MatrixTemp[ret], 1, 0, sizeof(float) * size, FilesavingTemp, 0, 0, 0);
		if(error != CL_SUCCESS) { printf("ERROR: 1  clEnqueueReadBuffer: input_ocl\n"); return -1; }
	}
	else
	{
		error = clEnqueueReadBuffer(command_queue, MatrixTemp[ret], 1, 0, sizeof(float) * size, swap, 0, 0, 0);
		if(error != CL_SUCCESS) { printf("ERROR: 1  clEnqueueReadBuffer: input_ocl\n"); return -1; }
	}

	//cl_float *MatrixOut = (cl_float *) clEnqueueMapBuffer(command_queue, MatrixTemp[ret], CL_TRUE, CL_MAP_READ, 0, sizeof(float) * size, 0, NULL, NULL, &error);
	//if (error != CL_SUCCESS) fatal_CL(error, __LINE__);

	syscall(END_PARALLEL_SECTION);
	#if M2S_CGM_OCL_SIM == 0
		p_time = (RDTSC() - p_start);
	#endif


	printf("Parallel Section Cycles %llu Kernel Cycles %llu\n", p_time, k_time);

	long long end_time = get_time();
	printf("Total time: %.3f seconds\n", ((float) (end_time - start_time)) / (1000*1000));


	//#define OUTPUT
#ifdef OUTPUT
	// Write final output to output file
    writeoutput(MatrixOut, grid_rows, grid_cols, ofile);
    
#endif

	//error = clEnqueueUnmapMemObject(command_queue, MatrixTemp[ret], (void *) MatrixOut, 0, NULL, NULL);
	//if (error != CL_SUCCESS) fatal_CL(error, __LINE__);
	

	clReleaseMemObject(MatrixTemp[0]);
	clReleaseMemObject(MatrixTemp[1]);
	clReleaseMemObject(MatrixPower);
	
    clReleaseContext(context);

	return 0;
}

