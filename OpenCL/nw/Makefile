#include ../../common/make.config
# this is for the m2s runtime
OPENCL_DIR = /home/stardica/Desktop/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/ 
OPENCL_LIB = $(OPENCL_DIR)/Release/ -lm2s-opencl

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL

#C compiler
CC = g++

CC_FLAGS = -g -O3 -Wall

EXE = nw

$(EXE): nw.c
	#$(CC) ${KERNEL_DIM} $(CC_FLAGS) -o $(EXE) nw.c -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) ${KERNEL_DIM} $(CC_FLAGS) -o $(EXE) nw.c -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl

clean:
	rm -f $(EXE)
