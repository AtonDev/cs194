#include "my_blurs.h"

using namespace std;

void fastest_blur(float* out, int n, float* frame, int* radii){
#pragma omp parallel for schedule(dynamic)
  for(int r=0; r<n; r++)
    for(int c=0; c<n; c++){
      int rd = radii[r*n+c];
      int num = (max(0,r-rd) - min(n-1, r+rd) - 1) * (max(0, c-rd) - min(n-1, c+rd) - 1);
      float avg = 0;
      __m128 avg4 = _mm_set1_ps(0);
      for(int r2=max(0,r-rd); r2<=min(n-1, r+rd); r2++) {
	for(int c2=max(0, c-rd); c2<=min(n-1, c+rd);){
          if (c2 + 4 <= min(n-1, c+rd)) {
	    __m128 vals = _mm_loadu_ps(frame + r2*n + c2);
            avg4 = _mm_add_ps(avg4, vals);
            c2 += 4;
          } else {
            avg += frame[r2*n+c2];
            c2 += 1;
          }
	}
      }
      float avg_a[4];
      _mm_storeu_ps(avg_a, avg4);
      for (int i = 0; i < 4; i++)
	avg += avg_a[i];
      out[r*n+c] = avg/num;
    }
}
