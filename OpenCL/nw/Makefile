SIMULATOR = 0
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

KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/nw/nw_new.cl.bin.GPU"'
# AMD_DIR
# this is for the amdapp sdk.
else
OPENCL_DIR = /opt/AMDAPP
OPENCL_INC = $(OPENCL_DIR)/include/ 
OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
LINKER_FLAGS= -pthread -ldl -lrt
CC_FLAGS = -g -O2
KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/nw/nw.cl.bin.GPU"'
endif


#

#C compiler
CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)

#CC_FLAGS = -g -O3 -Wall -m32

EXE = nw_cl

all: nw_cl

$(EXE): nw.c
	#$(CC) ${KERNEL_DIM} $(CC_FLAGS) -o $(EXE) nw.c -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) ${KERNEL_DIM} $(CC_FLAGS) -o $(EXE) nw.c -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

clean:
	rm -f $(EXE)
