#include ../../common/make.config

# M2S_DIR
# this is for the m2s runtime
OPENCL_DIR = /home/stardica/Desktop/runtime
OPENCL_INC = $(OPENCL_DIR)/src/runtime/include/ 
OPENCL_LIB = $(OPENCL_DIR)/Release_Static/ -lm2s-opencl

# AMD_DIR
# this is for the amdapp sdk.
#OPENCL_DIR = /opt/AMDAPP
#OPENCL_INC = $(OPENCL_DIR)/include/ 
#OPENCL_LIB = $(OPENCL_DIR)/lib/x86/ -lOpenCL


#KERNEL_PATH_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/OpenCL/srad/kernel/kernel_gpu_opencl.cl.bin.GPU"'
KERNEL_PATH_VAR='"/home/cgiles/Rodinia_Benchmarks/OpenCL/srad/kernel/kernel_gpu_opencl.cl.bin.GPU"'

#IMAGE_FILE_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/data/srad/image.pgm"'
IMAGE_FILE_VAR='"/home/cgiles/Rodinia_Benchmarks/data/srad/image.pgm"'

#IMAGE_OUTPUT_VAR='"/home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/data/srad/image_output.pgm"'
IMAGE_OUTPUT_VAR='"/home/cgiles/Rodinia_Benchmarks/data/srad/image_output.pgm"'


OCL_INC_DIR = -I$(OPENCL_INC)
OCL_LIB_DIR = -L$(OPENCL_LIB)

C_C = gcc -DKERNEL_PATH=$(KERNEL_PATH_VAR) -DIMAGE_FILE=$(IMAGE_FILE_VAR) -DIMAGE_OUTPUT=$(IMAGE_OUTPUT_VAR) -m32 
OCL_LIB = -lm2s-opencl
#OCL_LIB = -lOpenCL

all: ./a.out

./a.out:	./main.o \
		./kernel/kernel_gpu_opencl_wrapper.o \
		./util/opencl/opencl.o \
		./util/graphics/graphics.o \
		./util/graphics/resize.o \
		./util/timer/timer.o
		$(C_C)	./main.o \
		./kernel/kernel_gpu_opencl_wrapper.o \
		./util/opencl/opencl.o \
		./util/graphics/graphics.o \
		./util/graphics/resize.o \
		./util/timer/timer.o \
		-lm \
		$(OCL_LIB_DIR) \
		$(OCL_LIB) -o srad -static -pthread -ldl

# ======================================================================================================================================================150
#	MAIN FUNCTION
# ======================================================================================================================================================150

./main.o:	./main.h \
		./main.c
		$(C_C)	./main.c \
		-c \
		-o ./main.o \
		-O3

# ======================================================================================================================================================150
#	KERNELS
# ======================================================================================================================================================150

./kernel/kernel_gpu_opencl_wrapper.o:	./kernel/kernel_gpu_opencl_wrapper.h \
					./kernel/kernel_gpu_opencl_wrapper.c
					$(C_C)	./kernel/kernel_gpu_opencl_wrapper.c \
					-c \
					-o ./kernel/kernel_gpu_opencl_wrapper.o \
					-O3 \
					$(OCL_INC_DIR)

# ======================================================================================================================================================150
#	UTILITIES
# ======================================================================================================================================================150

./util/graphics/graphics.o:	./util/graphics/graphics.h \
				./util/graphics/graphics.c
				$(C_C)	./util/graphics/graphics.c \
				-c \
				-o ./util/graphics/graphics.o \
				-O3

./util/graphics/resize.o:	./util/graphics/resize.h \
				./util/graphics/resize.c
				$(C_C)	./util/graphics/resize.c \
				-c \
				-o ./util/graphics/resize.o \
				-O3

./util/timer/timer.o:	./util/timer/timer.h \
			./util/timer/timer.c
			$(C_C)	./util/timer/timer.c \
			-c \
			-o ./util/timer/timer.o \
			-O3

./util/opencl/opencl.o:	./util/opencl/opencl.h \
			./util/opencl/opencl.c
			$(C_C)	./util/opencl/opencl.c \
			-c \
			-o ./util/opencl/opencl.o \
			-O3 \
			$(OCL_INC_DIR)

clean:
	rm	./*.o srad\
		./kernel/*.o \
		./util/opencl/*.o \
		./util/graphics/*.o \
		./util/timer/*.o
