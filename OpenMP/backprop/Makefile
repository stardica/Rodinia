# C compiler
CC = gcc
CC_FLAGS = -g -fopenmp -O2 -m32

all: backprop

backprop: backprop.o facetrain.o imagenet.o backprop_kernel.o 
	$(CC) $(CC_FLAGS) backprop.o facetrain.o imagenet.o backprop_kernel.o -o backprop -lm -static

%.o: %.[ch]
	$(CC) $(CC_FLAGS) $< -c -static

facetrain.o: facetrain.c backprop.h
	$(CC) $(CC_FLAGS) facetrain.c -c -static
	
backprop.o: backprop.c backprop.h
	$(CC) $(CC_FLAGS) backprop.c -c -static

backprop_kernel.o: backprop_kernel.c backprop.h
	$(CC) $(CC_FLAGS) backprop_kernel.c -c -static

imagenet.o: imagenet.c backprop.h
	$(CC) $(CC_FLAGS) imagenet.c -c -static

clean:
	rm -f *.o *~ backprop backprop_cuda.linkinfo
