#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include "counters.h"


int main(int argc, char  *argv[])
{


  //initialize hw counter
  hwCounter_t  c1;
  c1.init = false;
  initTicks(c1);

  //get time before
  uint64_t t_before = getTicks(c1);

  //get the number of the user
  int n = atoi(argv[1]);

  //compute sum of first n intengers
  long long sum = 0;
  for (long long i = 0; i < n; ++i) {
    sum += i;
  }

  //print result of the sum 
  printf("Result of the sum: %llu.\n", sum);

  //get time after
  uint64_t elapsed = getTicks(c1) - t_before;


  //print time elapsed 
  printf("Time elapsed is %lu ticks.\n", elapsed);

  return 0;
}
