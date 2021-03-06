#include <pthread.h>
#include <cassert>

typedef struct
{
  double **a;
  double **b;
  double **c;
  int start;
  int end;
} worker_t;

void *matmuld_worker(void *arg)
{
  worker_t *t = static_cast<worker_t*>(arg);
  double **a = t->a;
  double **b = t->b;
  double **c = t->c;
  for(int i = t->start; i < t->end; i++)
    {
      for(int k = 0; k < 1024; k++)
	{
	  for(int j = 0; j < 1024; j++)
	    {
	      c[i][j] += a[i][k]*b[k][j];
	    }
	}
    }
}

void pthread_matmuld(double **a,
		     double **b,
		     double **c,
		     int nthr)
{
  /* CS194: use pthreads to launch 
   * matrix multply worker threads.
   *
   * The structure and worker function
   * are good hints...
   */
  pthread_t *thr = new pthread_t[nthr];
  worker_t *tInfo = new worker_t[nthr];

 
  int block = (1024 / nthr) + 1;
  // Create threads 
  for (int i = 0; i < nthr; i++) {    
    tInfo[i].start = block * i;
    if (block * (i + 1) >= 1024)
      tInfo[i].end = 1024;
    else
      tInfo[i].end = block * (i + 1);
    tInfo[i].a = a;
    tInfo[i].b = b;
    tInfo[i].c = c;
    pthread_create(&thr[i], NULL, matmuld_worker, (void *)(&tInfo[i]));  
  }
  
  for (int i = 0; i < nthr; i++) {
    pthread_join(thr[i], NULL);
    }
  
  delete [] thr;
  delete [] tInfo;
}
