#include ../../common/make.config
OPENCL_DIR = /home/stardica/Desktop/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/ 
OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL


CC = gcc
CC_FLAGS = -g -O2 -Wall -m32

EXE = hotspot

all: hotspot

$(EXE): hotspot.c hotspot.h OpenCL_helper_library.c OpenCL_helper_library.h
	#$(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) -I$(OPENCL_INC) OpenCL_helper_library.c hotspot.c -L$(OPENCL_LIB) -lOpenCL
	$(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) -I$(OPENCL_INC) OpenCL_helper_library.c hotspot.c -L$(OPENCL_LIB) -lm2s-opencl -static -lpthread -ldl -lrt

clean:
	rm -f $(EXE)
