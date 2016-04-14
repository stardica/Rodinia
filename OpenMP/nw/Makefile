# C compiler
CC = g++
CC_FLAGS = -g -fopenmp -m32

all: needle

needle: 
	$(CC) $(CC_FLAGS) needle.cpp -o needle -static -lpthread -ldl -lrt

clean:
	rm -f needle 
