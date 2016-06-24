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

INC = -I$(OPENCL_INC) -I../common
LIB = -L$(OPENCL_LIB)

#C compiler
CC = g++
CC_FLAGS = -g -O3 -Wall -m32

EXE = lud

all: lud

$(EXE): lud.cpp
	#$(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) lud.cpp common.c $(INC) $(LIB) -lOpenCL
	$(CC) $(KERNEL_DIM) $(CC_FLAGS) -o $(EXE) lud.cpp common.c $(INC) $(LIB) -lm2s-opencl -static -lpthread -ldl -lrt 

clean:
	rm -f $(EXE)
