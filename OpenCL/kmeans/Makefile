
#Kmeans OCL

SIMULATOR = 1
MEASURE = 0

ifeq ($(SIMULATOR), 1)
	OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
	OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
	OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/kmeans/kmeans_kernels.cl.bin.GPU"'
	CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	CC_FLAGS = -g -O2 -m32
	LINKER_FLAGS = -static -pthread -ldl -lrt
	DO_COMPILE = $(CC) $(KERNEL_DIM) $(CC_FLAGS) kmeans.cpp cluster.o getopt.o read_input.o kmeans_clustering.o rmse.o -o kmeans -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)
else
	# this is for the amdapp sdk.
	OPENCL_DIR = /opt/AMDAPP
	OPENCL_INC = $(OPENCL_DIR)/include/ 
	OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
	INTELPCM_INC = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/src/
	INTELPCM_LIB = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/Release_Static/ -lIntelPCM
	KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/kmeans/kmeans_kernels.cl.bin.GPU"'
	CC_FLAGS = -g -O2
	CC = g++ -std=c++11 -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	LINKER_FLAGS = -pthread -ldl -lrt
	DO_COMPILE = $(CC) $(KERNEL_DIM) $(CC_FLAGS) kmeans.cpp cluster.o getopt.o read_input.o kmeans_clustering.o rmse.o -o kmeans -I$(OPENCL_INC) -I$(INTELPCM_INC) -L$(INTELPCM_LIB) -L$(OPENCL_LIB) $(LINKER_FLAGS)
endif


all: kmeans

kmeans: cluster.o getopt.o read_input.o kmeans_clustering.o rmse.o
	$(DO_COMPILE)

%.o: %.[ch]
	$(CC) $(CC_FLAGS) $< -c -I$(INTELPCM_INC) -L$(INTELPCM_LIB)

cluster.o: cluster.c 
	$(CC) $(CC_FLAGS) cluster.c -c -I$(INTELPCM_INC) -L$(INTELPCM_LIB) -static 

getopt.o: getopt.c 
	$(CC) $(CC_FLAGS) getopt.c -c -I$(INTELPCM_INC) -L$(INTELPCM_LIB) -static

kmeans.o: kmeans.c 
	$(CC) $(CC_FLAGS) read_input.c -c -I$(INTELPCM_INC) -L$(INTELPCM_LIB) -static

rmse.o: rmse.c
	$(CC) $(CC_FLAGS) rmse.c -c -I$(INTELPCM_INC) -L$(INTELPCM_LIB) -static

clean:
	rm -f *.o *~ kmeans *.linkinfo *~ kmeans_cl
