#include ../../common/make.config

# this is for the m2s runtime
OPENCL_DIR = /home/stardica/Desktop/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/ 
OPENCL_LIB = $(OPENCL_DIR)/Release/ -lm2s-opencl

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL

#ifdef OUTPUT
#override OUTPUT = -DOUTPUT
#endif

OUTPUT = -DOUTPUT

C_C = gcc
OMP_LIB = -lgomp
OMP_FLAG = -fopenmp
OCL_INC = -I$(OPENCL_INC)

#OCL_LIB =	-L$(OPENCL_LIB) -lOpenCL
OCL_LIB =	-L$(OPENCL_LIB) -lm2s-opencl

all: a.out

# link objects together
a.out:		main.o \
			./kernel/kernel_gpu_opencl_wrapper.o \
			./util/num/num.o \
			./util/timer/timer.o \
			./util/opencl/opencl.o
	$(C_C)	main.o \
			./kernel/kernel_gpu_opencl_wrapper.o \
			./util/num/num.o \
			./util/timer/timer.o \
			./util/opencl/opencl.o \
			-lm \
			$(OCL_LIB) \
			-o lavaMD

# compile function files into objects
main.o:		main.h \
			main.c \
			./kernel/kernel_gpu_opencl_wrapper.h \
			./kernel/kernel_gpu_opencl_wrapper.c \
			./util/num/num.h \
			./util/num/num.c \
			./util/timer/timer.h \
			./util/timer/timer.c \
			./util/opencl/opencl.h \
			./util/opencl/opencl.c
	$(C_C)	$(OUTPUT) $(KERNEL_DIM) main.c \
			-c \
			-o main.o \
			-O3

./kernel/kernel_gpu_opencl_wrapper.o:	./kernel/kernel_gpu_opencl_wrapper.h \
										./kernel/kernel_gpu_opencl_wrapper.c
	$(C_C)	$(KERNEL_DIM)						./kernel/kernel_gpu_opencl_wrapper.c \
										$(OCL_INC) \
										-c \
										-o ./kernel/kernel_gpu_opencl_wrapper.o \
										-O3

./util/num/num.o:	./util/num/num.h \
					./util/num/num.c
	$(C_C)			./util/num/num.c \
					-c \
					-o ./util/num/num.o \
					-O3

./util/timer/timer.o:	./util/timer/timer.h \
						./util/timer/timer.c
	$(C_C)				./util/timer/timer.c \
						-c \
						-o ./util/timer/timer.o \
						-O3

./util/opencl/opencl.o:	./util/opencl/opencl.h \
						./util/opencl/opencl.c
	$(C_C)				./util/opencl/opencl.c \
						$(OCL_INC) \
						-c \
						-o ./util/opencl/opencl.o \
						-O3

# delete all object and executable files
clean:
	rm	*.o lavaMD \
		./kernel/*.o \
		./util/num/*.o \
		./util/timer/*.o \
		./util/opencl/*.o \
