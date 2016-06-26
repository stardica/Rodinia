#makefile

all: openmp

openmp: ex_particle_OPENMP_seq.c
	gcc -O3 -m32 -ffast-math -fopenmp ex_particle_OPENMP_seq.c -o particle_filter -lm -static -lpthread -ldl -lrt 


icc_openmp: ex_particle_OPENMP_seq.c
	icc -O3 -fast -openmp ex_particle_OPENMP_seq.c -o particle_filter -static -lpthread -ldl -lrt


clean:
	rm particle_filter
