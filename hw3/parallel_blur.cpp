#include "my_blurs.h"

using namespace std;

void parallel_blur(float* out, int n, float* frame, int* radii){
#pragma omp parallel for schedule(static, 4)
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
