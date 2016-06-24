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

#C compiler
CC = g++

SRC = bfs.cpp timer.cc

CC_FLAGS = -g -O2 -m32

EXE = bfs

all: release

release:$(SRC)
	#$(CC) $(CC_FLAGS)  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) $(CC_FLAGS)  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -static -ldl 

errmsg:$(SRC)
	#$(CC) $(CC_FLAGS)  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) $(CC_FLAGS)  -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -static

ptx:$(SRC)
	#$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL -D PTX_MSG
	$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -D PTX_MSG -static

profile:$(SRC)
	#$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL -D PROFILING
	$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -D PROFILING -static

res:$(SRC)
	#$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL -D RES_MSG
	$(CC) $(CC_FLAGS) -fopenmp $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -D RES_MSG -static

debug: $(SRC)
	#$(CC) $(CC_FLAGS) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) $(CC_FLAGS) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -static

run:
	./$(EXE)

clean: $(SRC)
	rm -f $(EXE) $(EXE).linkinfo result*
