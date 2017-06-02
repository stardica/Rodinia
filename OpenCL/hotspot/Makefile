
#hotspot

SIMULATOR = 1
MEASURE = 0


ifeq ($(SIMULATOR), 1)
	#include ../../common/make.config
	OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
	OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
	OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/hotspot/hotspot_kernel.cl.bin.GPU"'
	CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	CC_FLAGS = -g -O2 -Wall -m32
	LINKER_FLAGS= -static -pthread -ldl -lrt
	EXE = hotspot_cl
	DO_COMPILE = $(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) -I$(OPENCL_INC) OpenCL_helper_library.c hotspot.c -L$(OPENCL_LIB) $(LINKER_FLAGS)
else
	# this is for the amdapp sdk.
	OPENCL_DIR = /opt/AMDAPP
	OPENCL_INC = $(OPENCL_DIR)/include/ 
	OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
	INTELPCM_INC = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/src/
	INTELPCM_LIB = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/Release_Static/ -lIntelPCM
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/hotspot/hotspot_kernel.cl.bin.GPU"'
	CC = g++ -std=c++11 -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	CC_FLAGS = -g -O2 -Wall 
	LINKER_FLAGS= -pthread -ldl -lrt
	EXE = hotspot_cl
	DO_COMPILE = $(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) -I$(INTELPCM_INC) -I$(OPENCL_INC) OpenCL_helper_library.c hotspot.c -L$(OPENCL_LIB) -L$(INTELPCM_LIB) $(LINKER_FLAGS)
endif

all: hotspot_cl

$(EXE): hotspot.c hotspot.h OpenCL_helper_library.c OpenCL_helper_library.h
	$(DO_COMPILE)
	#$(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) -I$(OPENCL_INC) OpenCL_helper_library.c hotspot.c -L$(OPENCL_LIB) -lOpenCL
	#$(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) -I$(OPENCL_INC) OpenCL_helper_library.c hotspot.c -L$(OPENCL_LIB) -lm2s-opencl -static -lpthread -ldl -lrt

clean:
	rm -f $(EXE)
