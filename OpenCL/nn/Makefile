#include ../../common/make.config
# M2S_DIR
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

SRC = nearestNeighbor.cpp clutils.cpp utils.cpp

CC_FLAGS = -g -O2

EXE = nn

all: release

release:
	#$(CC) $(CC_FLAGS) -lOpenCL  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB)
	$(CC) $(CC_FLAGS) -lm2s-opencl  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB)
	
clean: 
	rm -f $(EXE) 
