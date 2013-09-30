#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>
#include <emmintrin.h>
#include <xmmintrin.h>

void parallel_blur(float* out, int n, float* frame, int* radii);
void vector_blur(float* out, int n, float* frame, int* radii);
void better_blur(float* out, int n, float* frame, int* radii);
void fastest_blur(float* out, int n, float* frame, int* radii);
