LOCAL_CC = gcc -g -O2 -Wall -m32
CC = gcc
CFLAGS = -lm -fopenmp -Wall -m32


all : nn

clean :
	rm -rf *.o nn

nn : nn_openmp.c
	$(CC) -o $@ $< $(LDFLAGS) $(CFLAGS) -static

hurricane_gen : hurricane_gen.c
	$(LOCAL_CC) -o $@ $< -lm

