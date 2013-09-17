#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include "counters.h"

#define STEPS 1048576
#define KBYTE 1000
#define MBYTE 1000000

using namespace std;

//forward declaration                                                             
int main(int argc, char *argv[]);
void shuffleArray(int array[], int size);
double pointerChase(int N, hwCounter_t c1);
void shuffleArray1c(int array[], int size);

int main(int argc, char *argv[]) {
  

  int step = 200 * KBYTE;
  int u_bound = 8 * MBYTE;
  int l_bound = 32 * KBYTE;

  //write in file
  ofstream myfile;
  myfile.open ("ptr_chase.csv");

  //init counter
  hwCounter_t counter;
  counter.init = false;
  initTicks(counter);

  // i is the size in BYTES
  int j = 1;
  for (int i = l_bound; i <= 2 * u_bound; i += j)
    {
      double size_in_kbytes = (double)i / (int)KBYTE;
      double avg_cycles_per_step = pointerChase(i, counter);
      myfile << (size_in_kbytes);
      myfile << ", ";
      myfile << avg_cycles_per_step;
      myfile << "\n";
      j *= 2;
    }
  myfile.close();

  return 0;
}

/* Returns the avg number of cycles per step
   for pointer chasing on an array of size BYTES in bytes. */
double pointerChase(int bytes, hwCounter_t c1) {
  //number of ints in array
  int N = bytes / sizeof(int);

  //init array
  int * array = (int *)malloc(sizeof(int) * N);
  shuffleArray1c(array, N);

  //start counter
  uint64_t start = getTicks(c1);

  //execute pointer chasing                                         
  int index = 0;
  int steps = STEPS;
  for (int i = 0; i < steps; ++i) {
    index = array[index];
  }

  //counter computation
  uint64_t end = getTicks(c1);
  uint64_t elapsed = end - start;
  double avg = (double)elapsed / steps;

  free(array);
  return avg;
}

void shuffleArray1c(int array[], int size)
{
  int * cycle_array = (int *)malloc(size * sizeof(int));
  shuffleArray(cycle_array, size);
  //corner case
  array[cycle_array[size - 1]] = cycle_array[0];

  //rest
  for (int i = 0; i < size - 1; i += 1) {
    array[cycle_array[i]] = cycle_array[i + 1];
  }
  free(cycle_array);

}


/* Initializes ARRAY of size SIZE to (0, ..., SIZE-1)   
   and shuffles it using Fisher-Yates algorithm. */
void shuffleArray(int array[], int size) {
  //initialize array (0, ..., size - 1)
  for (int i = 0; i < size; i += 1)
    array[i] = i;

  //seed the random number generator with the current time
  srand(time(NULL));

  //shuffle array using Fisher-Yates algorithm
  for (int i = 0; i < size; ++i) {
    int rand_index = rand() % size;
    int t = array[i];
    array[i] = array[rand_index];
    array[rand_index] = t;
  }

}
