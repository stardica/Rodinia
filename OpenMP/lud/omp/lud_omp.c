#include <stdio.h>
#include <omp.h>
#include <unistd.h>

#include "rdtsc.h"

#define BEGIN_PARALLEL_SECTION 325
#define END_PARALLEL_SECTION 326

unsigned long long p_start, p_end;

extern int omp_num_threads;

void lud_omp(float *a, int size)
{
     int i,j,k;
     float sum;
     
     printf("num of threads = %d\n", omp_num_threads);
     
     //simulator start stats collection
     p_start = rdtsc();
     syscall(BEGIN_PARALLEL_SECTION);

     for (i=0; i <size; i++)
     {
          omp_set_num_threads(omp_num_threads);

#pragma omp parallel for default(none) private(j,k,sum) shared(size,i,a) 
         
	  for (j=i; j <size; j++)
          {
		sum=a[i*size+j];
             	for (k=0; k<i; k++)
		{
			sum -= a[i*size+k]*a[k*size+j];
		}
                
		a[i*size+j]=sum;
         }

#pragma omp parallel for default(none) private(j,k,sum) shared(size,i,a) 
         
	   for (j=i+1;j<size; j++){
		sum=a[j*size+i];
		for (k=0; k<i; k++)
		{ 
			sum -=a[j*size+k]*a[k*size+i];
		}

		a[j*size+i]=sum/a[i*size+i];
         }
     }

     //simulator end stats collection
     syscall(END_PARALLEL_SECTION);
     p_end = rdtsc();

     printf("Parallel Section Cycles %llu\n", p_end - p_start);
}
