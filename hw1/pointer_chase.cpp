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
double ptr_chase(int N);
void cycle1permutation(int n_permutation[], int size);

/*Takes N as argument, N is the numb of ints..*/
int main(int argc, char *argv[]) {
  if (argc !=2) {
    printf("Requires one argument. \n");
    return 1;
  }
  int n = atoi(argv[1]);
  int step = KBYTE;
  int u_bound = MBYTE;

  //write in file
  ofstream myfile;
  myfile.open ("ptr_chase.csv");
  for (int i = 0; step * i <= u_bound; i *= 2)
    {
      myfile << (32 * (int)KBYTE + step * i);
      myfile << ", ";
      myfile << ptr_chase(32 * (int)KBYTE + step * i);
      myfile << "\n";
    }
  myfile.close();

  //print elapsed time
  printf("Avg ticks per cycle: %.2f\n", ptr_chase(n));
  printf("Size of int: %lu\n", sizeof(int));

  //good exit
  return 0;
}

/* Returns the avg number of cycles per step
   for pointer chasing on an array of size BYTES in bytes. */
double ptr_chase(int bytes)
{
  //number of ints in array
  int N = bytes / sizeof(int);

  //init counter
  hwCounter_t c1;
  c1.init = false;
  initTicks(c1);

  //init and shuffle array
  int * array = (int *)malloc(sizeof(int) * N);
  cycle1permutation(array, N);
  if (N < 11) {
    for (int i = 0; i < N; i++) {
      printf("%d, ", array[i]);
    }
    printf("\n");
  }
  //start counter
  uint64_t start = getTicks(c1);

  //execute pointer chasing
  int index = 0;
  int steps = STEPS;
  for (int i = 0; i < steps; ++i) {
      index = array[index];
    }

  //counter computation
  uint64_t elapsed = getTicks(c1) - start;
  double avg = (double)elapsed / STEPS;
}

/* Sets NPARRAY to an array representation of an
   N-Permuation with one cycle.*/
void cycle1permutation(int nparray[], int size)
{
  int *cycle = (int *)malloc(size * sizeof(int));
  // cycle representation of 1 cycle permutation of n
  shuffleArray(cycle, size);

  // array representation of permutation
  for (int i = 0; i < size - 1; i++)
    {
      nparray[cycle[i]] = cycle[i + 1];
    }
  nparray[cycle[size - 1]] = cycle[0];
}

/* Initializes ARRAY of size SIZE to (0, ..., SIZE-1)
   and shuffles it using Fisher-Yates algorithm. */
void shuffleArray(int array[], int size) {
  //initialize array (0, ..., size - 1)
  for (int i = 0; i < size; ++i)
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

