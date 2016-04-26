# C compiler
CC = g++
CC_FLAGS = -g -fopenmp -O2 -m32

all: hotspot

hotspot: 
	$(CC) $(CC_FLAGS) hotspot_openmp.cpp -o hotspot -static 

clean:
	rm -f hotspot
