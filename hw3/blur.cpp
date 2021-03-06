#include "my_blurs.h"

using namespace std;

double timestamp()
{
  struct timeval tv;
  gettimeofday (&tv, 0);
  return tv.tv_sec + 1e-6*tv.tv_usec;
}

// Simple Blur
void simple_blur(float* out, int n, float* frame, int* radii){
  for(int r=0; r<n; r++)
    for(int c=0; c<n; c++){
      int rd = radii[r*n+c];
      int num = 0;
      float avg = 0;
      for(int r2=max(0,r-rd); r2<=min(n-1, r+rd); r2++)
        for(int c2=max(0, c-rd); c2<=min(n-1, c+rd); c2++){
          avg += frame[r2*n+c2];
          num++;
        }
      out[r*n+c] = avg/num;
    }
}

// My Blur
void my_blur(float* out, int n, float* frame, int* radii){
  
}

int main(int argc, char *argv[])
{
  //Generate random radii
  srand(0);
  int n = 3000;
  int* radii = new int[n*n];
  for(int i=0; i<n*n; i++)
    radii[i] = 6*i/(n*n) + rand()%6;

  //Generate random frame
  float* frame = new float[n*n];
  for(int i=0; i<n*n; i++)
    frame[i] = rand()%256;

  //Blur using simple blur
  float* out = new float[n*n];
  double time = timestamp();
  simple_blur(out, n, frame, radii);
  time = timestamp() - time;

  //Blur using vector_blur
  float* out2 = new float[n*n];
  double time2 = timestamp();
  vector_blur(out2, n, frame, radii);
  time2 = timestamp() - time2;


  //Blur Using parallel blur
  printf("parallel_blur\n");
  printf("nthr\t time\n");
  for (int nthr = 1; nthr <= 16; nthr++) {
    omp_set_num_threads(nthr);
    float* out3 = new float[n*n];
    double time3 = timestamp();
    parallel_blur(out3, n, frame, radii);
    time3 = timestamp() - time3;
    printf("%d\t %.3f\n", nthr, time3);
  }

  //Blur using fastest_blur
  float* out3 = new float[n*n];
  omp_set_num_threads(16);
  double time3 = timestamp();
  fastest_blur(out3, n, frame, radii);
  time3 = timestamp() - time3;

  //Check result
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++){
      float dif = out[i*n+j] - out2[i*n+j];
      if(dif*dif>1.0f){
        printf("Your blur does not give the right result!\n");
        printf("For element (row, column) = (%d, %d):\n", i, j);
        printf("  Simple blur gives %.2f\n", out[i*n+j]);
        printf("  Your blur gives %.2f\n", out2[i*n+j]);
        exit(-1);
      }
  }

  //Delete
  delete[] radii;
  delete[] frame;
  delete[] out;
  delete[] out2;

  //Print out Time
  printf("Time needed for naive blur = %.3f seconds.\n", time);
  printf("Time needed for vector blur = %.3f seconds.\n", time2);
  printf("Time needed for fastest blur = %.3f seconds.\n", time3);
}
