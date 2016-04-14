# C compiler
CC = g++
CC_FLAGS = -g -fopenmp -O2 -m32

all: bfs

bfs: 
	$(CC) $(CC_FLAGS) bfs.cpp -o bfs -static

clean:
	rm -f bfs result.txt
