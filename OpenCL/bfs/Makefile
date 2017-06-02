
#BFS

SIMULATOR = 1
MEASURE = 0


ifeq ($(SIMULATOR), 1)
	OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
	OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
	OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl
	KERNEL_PATH_VAR ='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/bfs/bfs_kernels.cl.bin.GPU"'
	CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	CC_FLAGS = -g -O2 -m32 -fopenmp
	LINKER_FLAGS = -static -ldl
	EXE = bfs_cl
	SRC = bfs.cpp timer.cc
	DO_COMPILE = $(CC) $(CC_FLAGS) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)
else
	OPENCL_DIR = /opt/AMDAPP
	OPENCL_INC = $(OPENCL_DIR)/include/ 
	OPENCL_LIB = $(OPENCL_DIR)/lib/x86_64/ -lOpenCL
	INTELPCM_INC = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/src/
	INTELPCM_LIB = /home/stardica/Dropbox/CDA7919DoctoralResearch/IntelPCM/Release_Static/ -lIntelPCM
	KERNEL_PATH_VAR ='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/bfs/bfs_kernels.cl.bin.GPU"'
	CC = g++ -std=c++11 -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DM2S_CGM_OCL_SIM=$(SIMULATOR) -DM2S_CGM_OCL_MEASURE=$(MEASURE)
	CC_FLAGS = -g -O2 -fopenmp
	LINKER_FLAGS = -ldl
	EXE = bfs_cl
	SRC = bfs.cpp timer.cc
	DO_COMPILE = $(CC) $(CC_FLAGS) $(SRC) -o $(EXE) -I$(OPENCL_INC) -I$(INTELPCM_INC) -L$(INTELPCM_LIB) -L$(OPENCL_LIB) $(LINKER_FLAGS)
endif

all: release

release:$(SRC)
	$(DO_COMPILE)
	#$(CC) $(CC_FLAGS)  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -static -ldl 

errmsg:$(SRC)
	$(CC) $(CC_FLAGS)  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

ptx:$(SRC)
	#$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL -D PTX_MSG
	$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

profile:$(SRC)
	#$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL -D PROFILING
	$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

res:$(SRC)
	#$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL -D RES_MSG
	$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

debug: $(SRC)
	#$(CC) $(CC_FLAGS) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) $(CC_FLAGS) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(LINKER_FLAGS)

run:
	./$(EXE)

clean: $(SRC)
	rm -f $(EXE) $(EXE).linkinfo result*
