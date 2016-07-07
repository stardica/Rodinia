#include ../../common/make.config
# this is for the m2s runtime
OPENCL_DIR = /home/stardica/Desktop/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/ 
OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL

#KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/nw/nw_new.cl.bin.GPU"'
KERNEL_PATH_VAR='"/home/cgiles/Rodinia_Benchmarks/OpenCL/nw/nw_new.cl.bin.GPU"'

#C compiler
CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR)

CC_FLAGS = -g -O3 -Wall -m32

EXE = nw

all: nw

$(EXE): nw.c
	#$(CC) ${KERNEL_DIM} $(CC_FLAGS) -o $(EXE) nw.c -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) ${KERNEL_DIM} $(CC_FLAGS) -o $(EXE) nw.c -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -static -lpthread -ldl -lrt

clean:
	rm -f $(EXE)
