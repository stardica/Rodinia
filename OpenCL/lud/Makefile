

#LUD OCL

SIMULATOR = 1
MEASURE = 1

ifeq ($(SIMULATOR), 1)
	OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
	OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
	OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/lud/lud_kernel.cl.bin.GPU"'
	LINKER_FLAGS= -static -pthread -ldl -lrt
	CC_FLAGS = -g -O2 -m32
	CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	DO_COMPILE = $(CC) $(KERNEL_DIM) $(CC_FLAGS) -o lud_cl lud.cpp common.c -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

else
	# this is for the amdapp sdk.
	OPENCL_DIR = /opt/AMDAPP
	OPENCL_INC = $(OPENCL_DIR)/include/ 
	OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
	INTELPCM_INC = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/src/
	INTELPCM_LIB = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/Release_Static/ -lIntelPCM
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/lud/lud_kernel.cl.bin.GPU"'
	LINKER_FLAGS= -pthread -ldl -lrt
	CC_FLAGS = -g -O2
	CC = g++ -std=c++11 -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	DO_COMPILE = $(CC) $(KERNEL_DIM) $(CC_FLAGS) -o lud_cl lud.cpp common.c -I$(OPENCL_INC) -I$(INTELPCM_INC) -L$(OPENCL_LIB) -L$(INTELPCM_LIB) $(LINKER_FLAGS)
endif

#INC = -I$(OPENCL_INC) -I../common
#LIB = -L$(OPENCL_LIB)

all: lud_cl

lud_cl: lud.cpp
	$(DO_COMPILE)
	 
clean:
	rm -f $(EXE) lud_cl