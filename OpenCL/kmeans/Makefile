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

# C compiler
CC = g++
CC_FLAGS = -g -O2 

all: kmeans

kmeans: cluster.o getopt.o read_input.o kmeans_clustering.o rmse.o
	#$(CC) $(KERNEL_DIM) $(CC_FLAGS) -lOpenCL kmeans.cpp cluster.o getopt.o read_input.o kmeans_clustering.o rmse.o -o kmeans -I$(OPENCL_INC) -I$(OPENCL_DIR)/shared/inc/ -L$(OPENCL_LIB)
	$(CC) $(KERNEL_DIM) $(CC_FLAGS) -lm2s-opencl kmeans.cpp cluster.o getopt.o read_input.o kmeans_clustering.o rmse.o -o kmeans -I$(OPENCL_INC) -I$(OPENCL_DIR)/shared/inc/ -L$(OPENCL_LIB)

%.o: %.[ch]
	$(CC) $(CC_FLAGS) $< -c

cluster.o: cluster.c 
	$(CC) $(CC_FLAGS) cluster.c -c

getopt.o: getopt.c 
	$(CC) $(CC_FLAGS) getopt.c -c

kmeans.o: kmeans.c 
	$(CC) $(CC_FLAGS) read_input.c -c

rmse.o: rmse.c
	$(CC) $(CC_FLAGS) rmse.c -c

clean:
	rm -f *.o *~ kmeans *.linkinfo
