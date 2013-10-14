#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "counters.h"
#include <sys/time.h>
#include <time.h>

void opt_simd_sgemm(float *Y, float *A, float *B, int n);
void opt_scalar1_sgemm(float *Y, float *A, float *B, int n);
void opt_scalar0_sgemm(float *Y, float *A, float *B, int n);
void naive_sgemm(float *Y, float *A, float *B, int n);

int main(int argc, char *argv[])
{
  int n = (1<<10);
  int ops = (2*n-1)*n*n;
  float* A = new float[n*n];
  float* B = new float[n*n];
  float* Y = new float[n*n];
  
  
  //Initialize instruction counter 
  hwCounter_t c, c1 ;
  c.init = false;
  c1.init = false;

  initInsns(c);
  initTicks(c1);
  //Get current instruction count
  uint64_t time = getTicks(c1); 
  uint64_t count = getInsns(c);
  
  opt_scalar0_sgemm(Y, A, B, n);

  //Compute number of instructions executed
  uint64_t executed = getInsns(c) - count;
  uint64_t elapsed_c = getTicks(c1) -  time;

  //start time
  struct timeval tv;
  gettimeofday(&tv, 0);
  double start_time = tv.tv_sec + 1e-6 * tv.tv_usec;

  //routine to be tested
  opt_scalar0_sgemm(Y, A, B, n);

  //end time
  struct timeval tv1;
  gettimeofday(&tv1, 0);
  double end_time = tv1.tv_sec + 1e-6 * tv1.tv_usec;
  double elapsed_t = end_time - start_time;
  
  printf("Naive IPC: %.2f Flops: %.2f\n", (double)executed / elapsed_c, (double)ops / elapsed_t );




  delete [] A;
  delete [] B;
  delete [] Y;
}
