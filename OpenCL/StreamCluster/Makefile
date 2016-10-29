#include ../../common/make.config

# M2S_DIR
# star >> changed here!
# this is for the m2s runtime
OPENCL_DIR = /home/stardica/Dropbox/CDA7919DoctoralResearch/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
OPENCL_LIB = $(OPENCL_DIR)/Release_Static/

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL

KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/StreamCluster/streamcluster_kernels.cl.bin.GPU"'
#KERNEL_PATH_VAR='"/home/cgiles/Rodinia_Benchmarks/OpenCL/StreamCluster/streamcluster_kernels.cl.bin.GPU"'

#C compiler
CC = g++ -DKERNEL_PATH=$(KERNEL_PATH_VAR)

FLAGS = -O3 -m32
SRC = streamcluster.cpp
EXE = streamcluster_cl

all: release

release:$(SRC)
	#$(CC) -lOpenCL  $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB)	$(FLAGS)
	$(CC) -lm2s-opencl  $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(FLAGS) -lm2s-opencl -static -lpthread -ldl -lrt

errmsg:$(SRC)
	$(CC) -lOpenCL $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -D ERRMSG $(FLAGS)
	
ptx:$(SRC)
	$(CC) -lOpenCL $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -D PTX_MSG $(FLAGS)
	
res:$(SRC)
	$(CC) -lOpenCL $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -D RES_MSG $(FLAGS)
	
clean: $(SRC)
	rm -f $(EXE) $(EXE).linkinfo result*
