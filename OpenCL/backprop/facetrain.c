#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "backprop.h"
#include "omp.h"
#include <unistd.h>

extern char *strcpy();
extern void exit();

#define CHECK_POINT 327
//#define DEBUG_POINT 328

int layer_size = 0;

void backprop_face()
{
  BPNN *net;
  int i;
  float out_err, hid_err;
  net = bpnn_create(layer_size, 16, 1); // (16, 1 can not be changed)
  
  printf("Input layer size : %d\n", layer_size);
  load(net);
  //entering the training kernel, only one iteration

  syscall(CHECK_POINT);

  printf("Starting training kernel\n");
  bpnn_train_kernel(net, &out_err, &hid_err);

  bpnn_free(net);
  printf("\nFinish the training for one iteration\n");

}

int setup(int argc, char **argv)
{
	
  int seed;

  if (argc!=2)
  {
	  fprintf(stderr, "usage: backprop <num of input elements>\n");
	  exit(0);
  }

  layer_size = atoi(argv[1]);

  if (layer_size%16!=0)
  {
	  fprintf(stderr, "The number of input points must be divided by 16\n");
	  exit(0);
  }
  

  seed = 7;   
  bpnn_initialize(seed);

  backprop_face();

  exit(0);
}
