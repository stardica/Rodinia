#include "main.h"
#include <stdio.h>
#include <string.h>
//#include "/opt/AMDAPP/include/CL/cl.h"
#include "/home/stardica/Desktop/runtime/src/runtime/include/CL/cl.h"
//#include "../util/opencl/opencl.h"
#include "./kernel_gpu_opencl_wrapper.h"

#include "kernel_paths.h"


kernel_gpu_opencl_wrapper(image, Nr, Nc, Ne, niter, lambda, NeROI, iN, iS, jE, jW, iter, mem_size_i, mem_size_j){

	cl_int error;
	cl_uint num_platforms;
	cl_context context;
	char pbuf[100];
	cl_platform_id platform;
	size_t devices_size;
	cl_device_id device;

	error = clGetPlatformIDs(0, NULL, &num_platforms);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// Get the list of available platforms
	cl_platform_id *platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * num_platforms);
	error = clGetPlatformIDs(num_platforms, platforms, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	printf("\nclGetPlatformIDs\n");
	fflush(stdout);

	// Select the 1st platform
	platform = platforms[0];

	// Get the name of the selected platform and print it (if there are multiple platforms, choose the first one)

	error = clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, sizeof(pbuf), pbuf, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	
	printf("clGetPlatformInfo\n");
	fflush(stdout);

	// Create context properties for selected platform
	cl_context_properties context_properties[3] = {	CL_CONTEXT_PLATFORM, (cl_context_properties) platform, 0};

	// Create context for selected platform being GPU
	//star changed here.	
	context = clCreateContextFromType(context_properties, CL_DEVICE_TYPE_GPU, NULL, NULL, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// Get the number of devices (previousely selected for the context)

	error = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &devices_size);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// Get the list of devices (previousely selected for the context)
	cl_device_id *devices = (cl_device_id *) malloc(devices_size);
	error = clGetContextInfo(context, CL_CONTEXT_DEVICES, devices_size, devices, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// Select the first device (previousely selected for the context) (if there are multiple devices, choose the first one)
	device = devices[0];

	// Get the name of the selected device (previousely selected for the context) and print it
	error = clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(pbuf), pbuf, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	
	printf("Device: %s\n", pbuf);

	// Create a command queue
	cl_command_queue command_queue;
	command_queue = clCreateCommandQueue(context, device, 0, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	/*// Load kernel source code from file
	const char *source = load_kernel_source("./kernel/kernel_gpu_opencl.cl");
	size_t sourceSize = strlen(source);

	// Create the program
	cl_program program = clCreateProgramWithSource(context, 1, &source, &sourceSize, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}*/

	//CDA6908 changes star here
	//get binanry file
	FILE *filePointer = fopen(kernel_binary, "rb");
	if (filePointer == NULL)
	{
		printf("error opening kernel binary\n");
	}

	printf("read binary\n");
	fflush(stdout);

	// Determine the size of the binary
	size_t binarySize;
	fseek(filePointer, 0, SEEK_END);
	binarySize = ftell(filePointer);
	rewind(filePointer);

	unsigned char *programBinary;
	programBinary = (unsigned char *) malloc(sizeof(unsigned char[binarySize]));

	fread(programBinary, 1, binarySize, filePointer);
	fclose(filePointer);

	cl_int binaryStatus;
	cl_int err = 0;

	cl_program program = clCreateProgramWithBinary(context, 1, &device, &binarySize, (const unsigned char**)&programBinary, &binaryStatus, &err);
	if ((err != CL_SUCCESS) || (binaryStatus != CL_SUCCESS)) { printf("error in clCreateProgramWithBinary1\n");}

	printf("clCreateProgramWithBinary\n");
	fflush(stdout);

	// Compile the program
	error = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	printf("clBuildProgram\n");
	fflush(stdout);

	
	// Print warnings and errors from compilation
	static char log[65536]; 
	memset(log, 0, sizeof(log));

	//clGetProgramBuildInfo(program, &device, CL_PROGRAM_BUILD_LOG, sizeof(log)-1, log, NULL);

	printf("-----OpenCL Compiler Output-----\n");

	if (strstr(log,"warning:") || strstr(log, "error:")) 
		printf("<<<<\n%s\n>>>>\n", log);

	printf("--------------------------------\n");
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	//====================================================================================================100
	//	CREATE Kernels
	//====================================================================================================100

	// Extract kernel
	cl_kernel extract_kernel;
	extract_kernel = clCreateKernel(program, "extract_kernel", &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// Prepare kernel
	cl_kernel prepare_kernel;
	prepare_kernel = clCreateKernel(program, "prepare_kernel", &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// Reduce kernel
	cl_kernel reduce_kernel;
	reduce_kernel = clCreateKernel(program, "reduce_kernel", &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// SRAD kernel
	cl_kernel srad_kernel;
	srad_kernel = clCreateKernel(program, "srad_kernel", &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// SRAD2 kernel
	cl_kernel srad2_kernel;
	srad2_kernel = clCreateKernel(program, "srad2_kernel", &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// Compress kernel
	cl_kernel compress_kernel;
	compress_kernel = clCreateKernel(program, "compress_kernel", &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	int blocks_x;

	int mem_size; // matrix memory size
	mem_size = sizeof(fp) * Ne; // get the size of float representation of input IMAGE

	cl_mem d_I;
	d_I = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_iN;
	d_iN = clCreateBuffer(	context, CL_MEM_READ_WRITE, mem_size_i,NULL, &error );
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_iS;
	d_iS = clCreateBuffer(	context, CL_MEM_READ_WRITE, mem_size_i,NULL, &error );
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_jE;
	d_jE = clCreateBuffer(	context, CL_MEM_READ_WRITE, mem_size_j, NULL, &error );
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_jW;
	d_jW = clCreateBuffer(	context, CL_MEM_READ_WRITE, mem_size_j, NULL, &error );
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// allocate memory for derivatives
	cl_mem d_dN;
	d_dN = clCreateBuffer(	context, CL_MEM_READ_WRITE, mem_size, NULL, &error );
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_dS;
	d_dS = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_dW;
	d_dW = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_dE;
	d_dE = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error );
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// allocate memory for coefficient on DEVICE
	cl_mem d_c;
	d_c = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// allocate memory for partial sums on DEVICE

	cl_mem d_sums;
	d_sums = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	cl_mem d_sums2;
	d_sums2 = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	
	//COPY INPUT TO CPU
	error = clEnqueueWriteBuffer(command_queue, d_I, 1, 0, mem_size, image, 0, 0, 0);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// coordinates
	error = clEnqueueWriteBuffer(command_queue, d_iN, 1, 0, mem_size_i, iN, 0, 0, 0);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clEnqueueWriteBuffer(command_queue, d_iS, 1, 0, mem_size_i, iS, 0, 0, 0);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clEnqueueWriteBuffer(command_queue, d_jE, 1, 0, mem_size_j, jE, 0, 0, 0);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clEnqueueWriteBuffer(	command_queue, d_jW, 1, 0, mem_size_j, jW, 0, 0, 0);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	//KERNEL EXECUTION PARAMETERS
	
	// threads
	size_t local_work_size[1];
	local_work_size[0] = NUMBER_THREADS;

	// workgroups
	int blocks_work_size;
	size_t global_work_size[1];
	blocks_x = Ne/(int)local_work_size[0];
	if (Ne % (int)local_work_size[0] != 0)	//compensate for division remainder above by adding one grid
	{
		blocks_x = blocks_x + 1;
	}
	blocks_work_size = blocks_x;
	global_work_size[0] = blocks_work_size * local_work_size[0]; // define the number of blocks in the grid

	printf("max # of workgroups = %d, # of threads/workgroup = %d (ensure that device can handle)\n", \
		(int)(global_work_size[0]/local_work_size[0]), (int)local_work_size[0]);
	
	//Extract Kernel - SCALE IMAGE DOWN FROM 0-255 TO 0-1 AND EXTRACT

	//set arguments

	error = clSetKernelArg(extract_kernel, 0, 8, (void *) &Ne);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(	extract_kernel, 1, sizeof(cl_mem), (void *) &d_I);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	//launch kernel
	error = clEnqueueNDRangeKernel(command_queue, extract_kernel, 1, 0, global_work_size, local_work_size, 0, NULL, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	//Synchronization - wait for all operations in the command queue so far to finish
	

	// error = clFinish(command_queue);
	// if (error != CL_SUCCESS) 
	// fatal_CL(error, __LINE__);

	
	//WHAT IS CONSTANT IN COMPUTATION LOOP
	
	//Prepare Kernel
	
	error = clSetKernelArg(prepare_kernel, 0, 8, (void *) &Ne);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(prepare_kernel, 1, sizeof(cl_mem), (void *) &d_I);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(	prepare_kernel, 2, sizeof(cl_mem), (void *) &d_sums);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(	prepare_kernel, 3, sizeof(cl_mem), (void *) &d_sums2);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	
	//Reduce Kernel
	
	int blocks2_x;
	int blocks2_work_size;
	size_t global_work_size2[1];
	long no;
	int mul;
	int mem_size_single = sizeof(fp) * 1;
	fp total;
	fp total2;
	fp meanROI;
	fp meanROI2;
	fp varROI;
	fp q0sqr;

	error = clSetKernelArg(reduce_kernel, 0, 8, (void *) &Ne);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(reduce_kernel, 3, sizeof(cl_mem), (void *) &d_sums);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(reduce_kernel, 4, sizeof(cl_mem), (void *) &d_sums2);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	
	//SRAD Kernel
	error = clSetKernelArg(srad_kernel, 0, sizeof(fp), (void *) &lambda);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 1, sizeof(int), (void *) &Nr);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 2, sizeof(int), (void *) &Nc);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 3, 8, (void *) &Ne);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 4, sizeof(cl_mem), (void *) &d_iN);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 5, sizeof(cl_mem), (void *) &d_iS);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 6, sizeof(cl_mem), (void *) &d_jE);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 7, sizeof(cl_mem), (void *) &d_jW);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 8, sizeof(cl_mem), (void *) &d_dN);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 9, sizeof(cl_mem), (void *) &d_dS);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 10, sizeof(cl_mem), (void *) &d_dW);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 11, sizeof(cl_mem), (void *) &d_dE);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 13, sizeof(cl_mem), (void *) &d_c);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad_kernel, 14, sizeof(cl_mem), (void *) &d_I);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}


	//SRAD2 Kernel
	error = clSetKernelArg(srad2_kernel, 0, sizeof(fp), (void *) &lambda);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 1, sizeof(int), (void *) &Nr);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 2, sizeof(int), (void *) &Nc);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 3, 8, (void *) &Ne);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 4, sizeof(cl_mem), (void *) &d_iN);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 5, sizeof(cl_mem), (void *) &d_iS);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 6, sizeof(cl_mem), (void *) &d_jE);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 7, sizeof(cl_mem), (void *) &d_jW);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 8, sizeof(cl_mem), (void *) &d_dN);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 9, sizeof(cl_mem),	(void *) &d_dS);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 10, sizeof(cl_mem), (void *) &d_dW);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 11, sizeof(cl_mem), (void *) &d_dE);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 12, sizeof(cl_mem), (void *) &d_c);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(srad2_kernel, 13, sizeof(cl_mem), (void *) &d_I);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	//COMPUTATION

	printf("Iterations Progress: ");

	//execute main loop
	//do for the number of iterations input parameter
	for (iter=0; iter<niter; iter++)
	{										

		printf("%d ", iter);
		fflush(NULL);

		// Prepare kernel
		
		// launch kernel
		error = clEnqueueNDRangeKernel(command_queue, prepare_kernel, 1, NULL, 	global_work_size, local_work_size, 0, NULL, NULL);
		if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

		// synchronize
		// error = clFinish(command_queue);
		// if (error != CL_SUCCESS) 
			// fatal_CL(error, __LINE__);

		
		//Reduce Kernel - performs subsequent reductions of sums
		

		// initial values
		blocks2_work_size = blocks_work_size; // original number of blocks
		global_work_size2[0] = global_work_size[0]; 
		no = Ne; // original number of sum elements
		mul = 1; // original multiplier

		// loop
		while(blocks2_work_size != 0)
		{

			// set arguments that were uptaded in this loop
			error = clSetKernelArg(reduce_kernel, 1, 8, (void *) &no);
			if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

			error = clSetKernelArg(reduce_kernel, 2, sizeof(int), (void *) &mul);
			if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

			error = clSetKernelArg(reduce_kernel, 5, sizeof(int), (void *) &blocks2_work_size);
			if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

			// launch kernel
			error = clEnqueueNDRangeKernel(command_queue, reduce_kernel, 1, NULL, global_work_size2, local_work_size, 0, NULL, NULL);
			if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

			// synchronize
			// error = clFinish(command_queue);
			// if (error != CL_SUCCESS) 
				// fatal_CL(error, __LINE__);

			// update execution parameters
			no = blocks2_work_size;	// get current number of elements
			if(blocks2_work_size == 1)
			{
				blocks2_work_size = 0;
			}
			else
			{
				mul = mul * NUMBER_THREADS; // update the increment
				blocks_x = blocks2_work_size/(int)local_work_size[0]; // number of blocks
				if (blocks2_work_size % (int)local_work_size[0] != 0)  //compensate for division remainder above by adding one grid
				{
					blocks_x = blocks_x + 1;
				}
				blocks2_work_size = blocks_x;
				global_work_size2[0] = blocks2_work_size * (int)local_work_size[0];
			}

		}

		// copy total sums to device
		error = clEnqueueReadBuffer(command_queue, d_sums, CL_TRUE, 0, mem_size_single, &total, 0, 0, 0);
		if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

		error = clEnqueueReadBuffer(command_queue, d_sums2, CL_TRUE, 0, mem_size_single, &total2, 0, 0, 0);
		if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

		// calculate statistics
		
		meanROI	= total / (fp)(NeROI);	// gets mean (average) value of element in ROI
		meanROI2 = meanROI * meanROI;	//
		varROI = (total2 / (fp)(NeROI)) - meanROI2; // gets variance of ROI								
		q0sqr = varROI / meanROI2; // gets standard deviation of ROI

		// execute srad kernel
		// set arguments that were uptaded in this loop
		error = clSetKernelArg(srad_kernel, 12, sizeof(fp), (void *) &q0sqr);
		if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

		// launch kernel
		error = clEnqueueNDRangeKernel(command_queue, srad_kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);};

		// synchronize
		// error = clFinish(command_queue);
		// if (error != CL_SUCCESS) 
			// fatal_CL(error, __LINE__);

		// execute srad2 kernel
		
		//launch kernel
		error = clEnqueueNDRangeKernel(command_queue, srad2_kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

		// synchronize
		// error = clFinish(command_queue);
		// if (error != CL_SUCCESS) 
			// fatal_CL(error, __LINE__);

		// End
		

	}

	printf("\n");

	//Compress Kernel - SCALE IMAGE UP FROM 0-1 TO 0-255 AND COMPRESS
	
	//set parameters
	
	error = clSetKernelArg(compress_kernel, 0, 8, (void *) &Ne);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	error = clSetKernelArg(compress_kernel, 1, sizeof(cl_mem), (void *) &d_I);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// launch kernel
	
	error = clEnqueueNDRangeKernel(command_queue, compress_kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}


	//synchronize
	error = clFinish(command_queue);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	//End
	//COPY RESULTS BACK TO CPU
	

	error = clEnqueueReadBuffer(command_queue, d_I, CL_TRUE, 0, mem_size, image, 0, NULL, NULL);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// int i;
	// for(i=0; i<100; i++){
		// printf("%f ", image[i]);
	// }


	// OpenCL structures
	error = clReleaseKernel(extract_kernel);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseKernel(prepare_kernel);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseKernel(reduce_kernel);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseKernel(srad_kernel);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseKernel(srad2_kernel);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseKernel(compress_kernel);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseProgram(program);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// common_change
	error = clReleaseMemObject(d_I);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_c);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}	
	error = clReleaseMemObject(d_iN);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_iS);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_jE);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_jW);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_dN);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_dS);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_dE);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_dW);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_sums);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseMemObject(d_sums2);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

	// OpenCL structures
	error = clFlush(command_queue);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseCommandQueue(command_queue);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}
	error = clReleaseContext(context);
	if (error != CL_SUCCESS) {fatal_CL(error, __LINE__);}

}
