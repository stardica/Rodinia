#include ../../common/make.config

# M2S_DIR
# star >> changed here!
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


FLAGS = -O3
SRC = streamcluster.cpp
EXE = streamcluster

all: release

release:$(SRC)
	#$(CC) -lOpenCL  $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB)	$(FLAGS)
	$(CC) -lm2s-opencl  $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) $(FLAGS)

errmsg:$(SRC)
	$(CC) -lOpenCL $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -D ERRMSG $(FLAGS)
	
ptx:$(SRC)
	$(CC) -lOpenCL $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -D PTX_MSG $(FLAGS)
	
res:$(SRC)
	$(CC) -lOpenCL $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -D RES_MSG $(FLAGS)
	
clean: $(SRC)
	rm -f $(EXE) $(EXE).linkinfo result*
