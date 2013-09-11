#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
//#include "counters.h"

//forward declaration
int main(int argc, char *argv[]);
void shuffleArray(int array[], int size);

/*Takes N as argument.*/
int main(int argc, char *argv[]) {
  if (argc !=2) {
    printf("Requires one argument. \n");
    return 1;
  }
  //init number of steps
  int steps = 10;

  //init and shuffle array
  int N = atoi(argv[1]);
  int array[N];
  shuffleArray(array, N);

  //start counter

  //execute pointer chasing
  int index = 0;
  for (int i = 0; i < steps; ++i)
    index = array[index];

  //counter computation

  //good exit
  return 0;
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

