
#Backprop

SIMULATOR = 1
MEASURE = 0

ifeq ($(SIMULATOR), 1)
	OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
	OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
	OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/backprop/backprop_kernel.cl.bin.GPU"'
	CC_FLAGS = -g -O2 -m32
	CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	LINKER_FLAGS= -static -pthread -ldl -lrt
	DO_COMPILE = $(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop_ocl -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)
else
	# this is for the amdapp sdk.
	OPENCL_DIR = /opt/AMDAPP
	OPENCL_INC = $(OPENCL_DIR)/include/ 
	OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
	INTELPCM_INC = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/src/
	INTELPCM_LIB = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/Release_Static/ -lIntelPCM
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/backprop/backprop_kernel.cl.bin.GPU"'
	CC_FLAGS = -g -O2
	CC = g++ -std=c++11 -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	LINKER_FLAGS= -pthread -ldl -lrt
	DO_COMPILE = $(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop_ocl -I$(OPENCL_INC) -I$(INTELPCM_INC) -L$(INTELPCM_LIB) -L$(OPENCL_LIB) $(LINKER_FLAGS)
endif


# C compiler
#CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)

all: backprop

	#old code
	#$(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop -I$(OPENCL_INC) -L$(OPENCL_LIB)
backprop: backprop.o imagenet.o facetrain.o
		$(DO_COMPILE)
		#$(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop_ocl -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

%.o: %.[ch]
	$(CC) $(CC_FLAGS) $< -c
	
backprop.o: backprop.c 
	$(CC) $(CC_FLAGS) backprop.c -c -static
	
facetrain.o: facetrain.c
	$(CC) $(CC_FLAGS) facetrain.c -c -static

imagenet.o: imagenet.c
	$(CC) $(CC_FLAGS) imagenet.c -c -static

clean:
	rm -f *.o *~ backprop_ocl *.linkinfo *.out
