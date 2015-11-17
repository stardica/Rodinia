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

#OPENCL_DIR = /home/sc5nf/ati-stream-sdk-v2.2-lnx64/
#INC = $(OPENCL_DIR)/include/ 
#LIB = $(OPENCL_DIR)/lib/x86_64/

FLAGS = -O2
SRC = euler3d.cpp

EXE = euler3d

all: release

release:$(SRC)
	#$(CC) $(KERNEL_DIM) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL $(FLAGS)
	$(CC) $(KERNEL_DIM) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl $(FLAGS) -static -lpthread -ldl -lrt

errmsg:$(SRC)
	$(CC) $(KERNEL_DIM) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL -D ERRMSG $(FLAGS) -static

ptx:$(SRC)
	$(CC) $(KERNEL_DIM) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB)  -lOpenCL -D PTX_MSG $(FLAGS) -static

res:$(SRC)
	$(CC) $(KERNEL_DIM) $(SRC) -o $(EXE) -I$(OPENCL_INC) -L$(OPENCL_LIB)  -lOpenCL -D RES_MSG $(FLAGS) -static

clean: $(SRC)
	rm -f $(EXE) $(EXE).linkinfo result*
