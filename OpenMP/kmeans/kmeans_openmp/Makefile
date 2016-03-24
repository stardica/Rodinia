# C compiler
CC = gcc
CC_FLAGS = -g -fopenmp -O2 -m32

all: kmeans

kmeans: cluster.o getopt.o kmeans.o kmeans_clustering.o 
	$(CC) $(CC_FLAGS) cluster.o getopt.o kmeans.o kmeans_clustering.o  -o kmeans -static

%.o: %.[ch]
	$(CC) $(CC_FLAGS) $< -c -static

cluster.o: cluster.c 
	$(CC) $(CC_FLAGS) cluster.c -c -static
	
getopt.o: getopt.c 
	$(CC) $(CC_FLAGS) getopt.c -c -static
	
kmeans.o: kmeans.c 
	$(CC) $(CC_FLAGS) kmeans.c -c -static

kmeans_clustering.o: kmeans_clustering.c kmeans.h
	$(CC) $(CC_FLAGS) kmeans_clustering.c -c -static

clean:
	rm -f *.o *~ kmeans 
