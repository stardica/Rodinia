#include ../../common/make.config

# M2S_DIR
# this is for the m2s runtime
OPENCL_DIR = /home/stardica/Desktop/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/ 
OPENCL_LIB = $(OPENCL_DIR)/Release/ -lm2s-opencl

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL

CXX = g++
DEBUGFLAGS = -g

# You will need to modify the following 2 lines
# by inserting the correct path to the OpenCL
# lib and inc directories.
CXXFLAGS = -I$(OPENCL_INC) -c $(DEBUGFLAGS)
LINKFLAGS = -L$(OPENCL_LIB) 

all: a.out

# link objects(binaries) together
a.out: main.o OpenCL.o
	$(CXX) main.o OpenCL.o $(LINKFLAGS) -o pathfinder

# compile main function file into object (binary)
main.o: main.cpp OpenCL.cpp kernels.cl
	$(CXX) $(CXXFLAGS) main.cpp OpenCL.cpp

clean:
	@rm -f pathfinder *.o *.out *~ *\#

	#clear