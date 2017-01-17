SIMULATOR = 1
MEASURE = 1

#include ../../common/make.config
# M2S_DIR
# star >> changed here!
# this is for the m2s runtime

ifeq ($(SIMULATOR), 1)
OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl
LINKER_FLAGS= -static -pthread -ldl -lrt
CC_FLAGS = -g -O2 -m32
# AMD_DIR
# this is for the amdapp sdk.
else
OPENCL_DIR = /opt/AMDAPP
OPENCL_INC = $(OPENCL_DIR)/include/ 
OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
LINKER_FLAGS= -pthread -ldl -lrt
CC_FLAGS = -g -O2
endif

KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/backprop/backprop_kernel.cl.bin.GPU"'
#KERNEL_PATH_VAR='"/home/cgiles/Rodinia_Benchmarks/OpenCL/backprop/backprop_kernel.cl.bin.GPU"'

# C compiler
CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)

all: backprop

	#old code
	#$(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop -I$(OPENCL_INC) -L$(OPENCL_LIB)
backprop: backprop.o imagenet.o facetrain.o
		$(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop_ocl -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

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
