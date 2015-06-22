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

C_C = gcc
#OCL_LIB = -lOpenCL
OCL_LIB = -lm2s-opencl

all: myocyte

./myocyte:	./main.o \
			./kernel/kernel_gpu_opencl_wrapper.o \
			./util/file/file.o \
			./util/num/num.o \
			./util/timer/timer.o \
			./util/opencl/opencl.o
	$(C_C)	./main.o \
			./kernel/kernel_gpu_opencl_wrapper.o \
			./util/file/file.o \
			./util/num/num.o \
			./util/timer/timer.o \
			./util/opencl/opencl.o \
			-I$(OPENCL_INC) \
			-lm \
			-L$(OPENCL_LIB) \
                        $(OCL_LIB) \
                        -o myocyte
# ========================================================================================================================================================================================================200
#	OBJECTS (COMPILE SOURCE FILES INTO OBJECTS)
# ========================================================================================================================================================================================================200

# ======================================================================================================================================================150
#	MAIN FUNCTION
# ======================================================================================================================================================150

./main.o:	./common.h \
			./main.h \
			./main.c
	$(C_C)	./main.c \
			-c \
			-o ./main.o \
			-O3 \
			-g

# ======================================================================================================================================================150
#	KERNELS
# ======================================================================================================================================================150

./kernel/kernel_gpu_opencl_wrapper.o:	./common.h \
										./kernel/kernel_gpu_opencl_wrapper.h \
										./kernel/kernel_gpu_opencl_wrapper.c \
										./kernel/solver.c \
										./kernel/embedded_fehlberg_7_8.c \
										./kernel/master.c \
										./kernel/kernel_gpu_opencl.cl
	$(C_C)	./kernel/kernel_gpu_opencl_wrapper.c \
			-c \
			-o ./kernel/kernel_gpu_opencl_wrapper.o \
			-I$(OPENCL_INC) \
			-O3

# ======================================================================================================================================================150
#	UTILITIES
# ======================================================================================================================================================150

./util/file/file.o:	./common.h \
					./util/file/file.h \
					./util/file/file.c
	$(C_C)	./util/file/file.c \
			-c \
			-o ./util/file/file.o \
			-O3

./util/num/num.o:	./common.h \
					./util/num/num.h \
					./util/num/num.c
	$(C_C)	./util/num/num.c \
			-c \
			-o ./util/num/num.o \
			-O3

./util/timer/timer.o:	./common.h \
						./util/timer/timer.h \
						./util/timer/timer.c
	$(C_C)	./util/timer/timer.c \
			-c \
			-o ./util/timer/timer.o \
			-O3

./util/opencl/opencl.o:	./common.h \
						./util/opencl/opencl.h \
						./util/opencl/opencl.c
	$(C_C)	-I$(OPENCL_INC) \
		./util/opencl/opencl.c \
			-c \
			-o ./util/opencl/opencl.o \
			-O3

# ======================================================================================================================================================150
#	END
# ======================================================================================================================================================150

# ========================================================================================================================================================================================================200
#	DELETE
# ========================================================================================================================================================================================================200

clean:
	rm	*.o myocyte \
		./kernel/*.o \
		./util/file/*.o \
		./util/timer/*.o \
		./util/opencl/*.o \
                ./util/num/*.o \
                ./output.txt