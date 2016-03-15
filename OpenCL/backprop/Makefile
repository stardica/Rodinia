#include ../../common/make.config
# M2S_DIR
# star >> changed here!
# this is for the m2s runtime
OPENCL_DIR = /home/stardica/Desktop/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/
OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL

# C compiler
CC = g++
CC_FLAGS = -g -O2 -m32

all: backprop

backprop: backprop.o imagenet.o facetrain.o
	#$(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop -I$(OPENCL_INC) -L$(OPENCL_LIB) -lOpenCL
	$(CC) $(CC_FLAGS) backprop_ocl.cpp backprop.o imagenet.o facetrain.o -o backprop_ocl -I$(OPENCL_INC) -L$(OPENCL_LIB) -lm2s-opencl -static -pthread -ldl -lrt

%.o: %.[ch]
	$(CC) $(CC_FLAGS) $< -c
	
backprop.o: backprop.c 
	$(CC) $(CC_FLAGS) backprop.c -c -static
	
facetrain.o: facetrain.c
	$(CC) $(CC_FLAGS) facetrain.c -c -static

imagenet.o: imagenet.c
	$(CC) $(CC_FLAGS) imagenet.c -c -static

clean:
	rm -f *.o *~ backprop_ocl *.linkinfo
