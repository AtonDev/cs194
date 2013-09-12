#include <sys/time.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>

//forward declarations
int main(int argc, char *argv[]);
void array_copy(int from_array[], int to_array[], int size_nbytes);


int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Requires one argument, namely the byte size of the array.");
    return 1;
  }

  int byte_size = atoi(argv[1]);
  int n = byte_size/sizeof(int);
  int origin[n];
  int destination[n];

  //init origin array
  for (int i = 0; i < n; i++)
    origin[i] = i;

  //cache warming
  for (int i = 0; i < 1; i ++)
    array_copy(origin, destination, byte_size);


  //start time
  struct timeval tv;
  gettimeofday(&tv, 0);
  double start_time = tv.tv_sec + 1e-6 * tv.tv_usec;

  //routine to be tested
  array_copy(origin, destination, byte_size);


  //end time
  struct timeval tv1;
  gettimeofday(&tv1, 0);
  double end_time = tv1.tv_sec + 1e-6 * tv1.tv_usec;
  double elapsed = end_time - start_time;

  //print avg number of Mbps
  double avg = (8 * (double)byte_size  / 1000000)/elapsed;
  printf("Memory bandwidth is %.2f Mbps\n", avg);

  //execution successfull
  return 0;
}

void array_copy(int from_array[], int to_array[], int size_nbytes)
{
  int size = size_nbytes / sizeof(int);
  for (int i = 0; i < size; i += 1)
      to_array[i] = from_array[i];
}
