#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include <sched.h>

#include "backprop.h"
#include "rdtsc.h"

#define BEGIN_PARALLEL_SECTION 325
#define END_PARALLEL_SECTION 326
#define CHECK_POINT 327

#define ITR 1000

unsigned long long p_start, p_end;

cpu_set_t  mask;

inline void assignToThisCore(int core_id)
{
    CPU_ZERO(&mask);
    CPU_SET(core_id, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);
}

long long ave_p_time = 0;
long long loop_count = 0;



////////////////////////////////////////////////////////////////////////////////

extern void bpnn_layerforward(float *l1, float *l2, float **conn, int n1, int n2);

extern void bpnn_output_error(float *delta, float *target, float *output, int nj, float *err);

extern void bpnn_hidden_error(float *delta_h, int nh, float *delta_o, int no, float **who, float *hidden, float *err);

extern void bpnn_adjust_weights(float *delta, int ndelta, float *ly, int nly, float **w, float **oldw);


extern int setup(int argc, char** argv);

extern float **alloc_2d_dbl(int m, int n);

extern float squash(float x);

double gettime() {
  struct timeval t;
  gettimeofday(&t,NULL);
  return t.tv_sec+t.tv_usec*1e-6;
}

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int
main( int argc, char** argv) 
{
	//assignToThisCore(0);//assign to core 0,1,2,...

	//int i = 0;

	//for(i = 0; i < ITR; i++)
	//{
		//loop_count++;
		//printf("running loop %d\n", i);
		setup(argc, argv);
	//}

	return 1;
}


void bpnn_train_kernel(BPNN *net, float *eo, float *eh)
{
  int in, hid, out;
  float out_err, hid_err;
  
  in = net->input_n;
  hid = net->hidden_n;
  out = net->output_n;

  syscall(CHECK_POINT);
   
  printf("Performing CPU computation\n");

  //simulator start stats collection
  p_start = rdtsc();
  syscall(BEGIN_PARALLEL_SECTION);

  bpnn_layerforward(net->input_units, net->hidden_units,net->input_weights, in, hid);
  bpnn_layerforward(net->hidden_units, net->output_units, net->hidden_weights, hid, out);
  bpnn_output_error(net->output_delta, net->target, net->output_units, out, &out_err);
  bpnn_hidden_error(net->hidden_delta, hid, net->output_delta, out, net->hidden_weights, net->hidden_units, &hid_err);  
  bpnn_adjust_weights(net->output_delta, out, net->hidden_units, hid, net->hidden_weights, net->hidden_prev_weights);
  bpnn_adjust_weights(net->hidden_delta, hid, net->input_units, in, net->input_weights, net->input_prev_weights);

  syscall(END_PARALLEL_SECTION);
  p_end = rdtsc();

  /*running ave = ((old count * old data) + next data) / next count*/
  //ave_p_time = (((loop_count - 1) * ave_p_time) + (p_end - p_start)) / loop_count;
  printf("Ave time %llu\n", ave_p_time);
  printf("Parallel Section cycles %llu\n", p_end - p_start);

}
