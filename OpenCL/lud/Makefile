SIMULATOR = 0
MEASURE = 1

#include ../../common/make.config
# this is for the m2s runtime
ifeq ($(SIMULATOR), 1)
OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl
LINKER_FLAGS= -static -pthread -ldl -lrt
CC_FLAGS = -g -O2 -m32

else
# AMD_DIR
# this is for the amdapp sdk.
OPENCL_DIR = /opt/AMDAPP
OPENCL_INC = $(OPENCL_DIR)/include/ 
OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
LINKER_FLAGS= -pthread -ldl -lrt
CC_FLAGS = -g -O2
endif

INC = -I$(OPENCL_INC) -I../common
LIB = -L$(OPENCL_LIB)

KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/lud/lud_kernel.cl.bin.GPU"'
#KERNEL_PATH_VAR='"/home/cgiles/Rodinia_Benchmarks/OpenCL/lud/lud_kernel.cl.bin.GPU"'

CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)

#C compiler
#CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR)
#CC_FLAGS = -g -O2 -m32#-g -O3 -Wall -m32

all: lud_cl

#$(KERNEL_DIM)

lud_cl: lud.cpp
	$(CC) $(CC_FLAGS) -o lud_cl lud.cpp common.c -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)
	#$(CC) $(CC_FLAGS) -o lud_cl lud.cpp common.c $(INC) $(LIB) -lm2s-opencl -static -lpthread -ldl -lrt
	#$(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) lud.cpp common.c $(INC) $(LIB) -lOpenCL
	 

clean:
	rm -f $(EXE) lud_cl