#include <sys/time.h>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <emmintrin.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>

#define KBYTE 1000
#define MBYTE 1000000

using namespace std;

//forward declarations
int main(int argc, char *argv[]);
void array_copy(int from_array[], int to_array[], int size_nbytes);
void simd_memcpy(void *dst, void *src, size_t nbytes);
void simd_memcpy_cache(void *dst, void *src, size_t nbytes);
double runRoutine1(int size_bytes);
double runRoutine2(int size_bytes);
double runRoutine3(int size_bytes);

int main(int argc, char *argv[])
{
  int u_bound = 8 * MBYTE;
  int l_bound = 32 * KBYTE;

  //write in file
  ofstream myfile;
  myfile.open ("my_routine.csv");
  // i is the size in BYTES
  int j = 1;
  for (int i = l_bound; i <= 2 * u_bound; i += j)
    {
      double size_in_kbytes = (double)i / (int)KBYTE;
      double bandwidth = runRoutine1(i);
      myfile << (size_in_kbytes);
      myfile << ", ";
      myfile << bandwidth;
      myfile << "\n";
      j *= 2;
    }
  myfile.close();
 
  //write in file                                                                                 
  ofstream file2;
  file2.open ("simd.csv");
  // i is the size in BYTES
  j = 1;
  for (int i = l_bound; i <= 2 * u_bound; i += j)
    {
      double size_in_kbytes = (double)i / (int)KBYTE;
      double bandwidth = runRoutine2(i);
      file2 << (size_in_kbytes);
      file2 << ", ";
      file2 << bandwidth;
      file2 << "\n";
      j *= 2;
    }
  file2.close();


  ofstream file3;
  file3.open ("simd_cache.csv");
  // i is the size in BYTES
  j = 1;
  for (int i = l_bound; i <= 2 * u_bound; i += j)
    {
      double size_in_kbytes = (double)i / (int)KBYTE;
      double bandwidth = runRoutine3(i);
      file3 << (size_in_kbytes);
      file3 << ", ";
      file3 << bandwidth;
      file3 << "\n";
      j *= 2;
    }
  file3.close();

  //execution successfull
  return 0;
}

double runRoutine1(int size_bytes)
{
  int n = sizeof(int);
  int ints = size_bytes / n;
  int * origin = (int *)malloc(size_bytes);
  int * destination = (int *)malloc(size_bytes);

  //init origin array
  for (int i = 0; i < ints; i += 1) {
    origin[i] = i;
  }

  //cache warming
  for (int i = 0; i < 3; i ++) {
    array_copy(origin, destination, size_bytes);
  }

  //start time
  struct timeval tv;
  gettimeofday(&tv, 0);
  double start_time = tv.tv_sec + 1e-6 * tv.tv_usec;

  //routine to be tested
  array_copy(origin, destination, size_bytes);


  //end time
  struct timeval tv1;
  gettimeofday(&tv1, 0);
  double end_time = tv1.tv_sec + 1e-6 * tv1.tv_usec;
  double elapsed = end_time - start_time;

  //print avg number of Mbps
  double avg = (8 * (double)size_bytes  / 1000000)/elapsed;

  return avg;
}

double runRoutine2(int size_bytes)
{
  int n = sizeof(int);
  int ints = size_bytes / n;
  int * origin = (int *)malloc(size_bytes);
  int * destination = (int *)malloc(size_bytes);

  //init origin array                                                                             
  for (int i = 0; i < ints; i += 1) {
    origin[i] = i;
  }

  //cache warming                                                                                 
  for (int i = 0; i < 3; i ++) {
    array_copy(origin, destination, size_bytes);
  }

  //start time                                                                                    
  struct timeval tv;
  gettimeofday(&tv, 0);
  double start_time = tv.tv_sec + 1e-6 * tv.tv_usec;

  //routine to be tested                                                                          
  simd_memcpy(origin, destination, size_bytes);


  //end time                                                                                      
  struct timeval tv1;
  gettimeofday(&tv1, 0);
  double end_time = tv1.tv_sec + 1e-6 * tv1.tv_usec;
  double elapsed = end_time - start_time;

  //print avg number of Mbps                                                                      
  double avg = (8 * (double)size_bytes  / 1000000)/elapsed;

  return avg;
}

double runRoutine3(int size_bytes)
{
  int n = sizeof(int);
  int ints = size_bytes / n;
  int * origin = (int *)malloc(size_bytes);
  int * destination = (int *)malloc(size_bytes);

  for (int i = 0; i < ints; i += 1) {
    origin[i] = i;
  }

  for (int i = 0; i < 3; i ++) {
    array_copy(origin, destination, size_bytes);
  }

  struct timeval tv;
  gettimeofday(&tv, 0);
  double start_time = tv.tv_sec + 1e-6 * tv.tv_usec;

  simd_memcpy_cache(origin, destination, size_bytes);


  struct timeval tv1;
  gettimeofday(&tv1, 0);
  double end_time = tv1.tv_sec + 1e-6 * tv1.tv_usec;
  double elapsed = end_time - start_time;

  double avg = (8 * (double)size_bytes  / 1000000)/elapsed;

  return avg;
}

void array_copy(int from_array[], int to_array[], int size_nbytes)
{
  int size = size_nbytes / sizeof(int);
  for (int i = 0; i < size; i += 1)
      to_array[i] = from_array[i];
}


void simd_memcpy(void *dst, void *src, size_t nbytes)
{
  size_t i;

  size_t ilen = nbytes/sizeof(int);
  size_t ilen_sm = ilen - ilen%16;

  char *cdst=(char*)dst;
  char *csrc=(char*)src;

  int * idst=(int*)dst;
  int * isrc=(int*)src;

  __m128i l0,l1,l2,l3;

  _mm_prefetch((__m128i*)&isrc[0], _MM_HINT_NTA);
  _mm_prefetch((__m128i*)&isrc[4], _MM_HINT_NTA);
  _mm_prefetch((__m128i*)&isrc[8], _MM_HINT_NTA);
  _mm_prefetch((__m128i*)&isrc[12], _MM_HINT_NTA);
  
  for(i=0;i<ilen_sm;i+=16)
    {
      l0 =  _mm_load_si128((__m128i*)&isrc[i+0]);
      l1 =  _mm_load_si128((__m128i*)&isrc[i+4]);
      l2 =  _mm_load_si128((__m128i*)&isrc[i+8]);
      l3 =  _mm_load_si128((__m128i*)&isrc[i+12]);
    
      _mm_prefetch((__m128i*)&isrc[i+16], _MM_HINT_NTA);
      _mm_prefetch((__m128i*)&isrc[i+20], _MM_HINT_NTA);
      _mm_prefetch((__m128i*)&isrc[i+24], _MM_HINT_NTA);
      _mm_prefetch((__m128i*)&isrc[i+28], _MM_HINT_NTA);

      _mm_stream_si128((__m128i*)&idst[i+0],  l0);
      _mm_stream_si128((__m128i*)&idst[i+4],  l1);
      _mm_stream_si128((__m128i*)&idst[i+8],  l2);
      _mm_stream_si128((__m128i*)&idst[i+12], l3);

    }

  for(i=ilen_sm;i<ilen;i++)
    {
      idst[i] = isrc[i];
    }

  for(i=(4*ilen);i<nbytes;i++)
    {
      cdst[i] = csrc[i];
    }
}


void simd_memcpy_cache(void *dst, void *src, size_t nbytes)
{
  size_t i;
  size_t sm = nbytes - nbytes%sizeof(int);
  size_t ilen = nbytes/sizeof(int);
  size_t ilen_sm = ilen - ilen%16;

  //printf("nbytes=%zu,ilen=%zu,ilen_sm=%zu\n",
  //nbytes,ilen,ilen_sm);


  char *cdst=(char*)dst;
  char *csrc=(char*)src;

  int * idst=(int*)dst;
  int * isrc=(int*)src;

  __m128i l0,l1,l2,l3;

  _mm_prefetch((__m128i*)&isrc[0], _MM_HINT_T0);
  _mm_prefetch((__m128i*)&isrc[4], _MM_HINT_T0);
  _mm_prefetch((__m128i*)&isrc[8], _MM_HINT_T0);
  _mm_prefetch((__m128i*)&isrc[12], _MM_HINT_T0);
  
  for(i=0;i<ilen_sm;i+=16)
    {
      l0 =  _mm_load_si128((__m128i*)&isrc[i+0]);
      l1 =  _mm_load_si128((__m128i*)&isrc[i+4]);
      l2 =  _mm_load_si128((__m128i*)&isrc[i+8]);
      l3 =  _mm_load_si128((__m128i*)&isrc[i+12]);
    
      _mm_prefetch((__m128i*)&isrc[i+16], _MM_HINT_T0);
      _mm_prefetch((__m128i*)&isrc[i+20], _MM_HINT_T0);
      _mm_prefetch((__m128i*)&isrc[i+24], _MM_HINT_T0);
      _mm_prefetch((__m128i*)&isrc[i+28], _MM_HINT_T0);

      _mm_store_si128((__m128i*)&idst[i+0],  l0);
      _mm_store_si128((__m128i*)&idst[i+4],  l1);
      _mm_store_si128((__m128i*)&idst[i+8],  l2);
      _mm_store_si128((__m128i*)&idst[i+12], l3);

    }

  for(i=ilen_sm;i<ilen;i++)
    {
      idst[i] = isrc[i];
    }

  for(i=(ilen*4);i<nbytes;i++)
    {
      cdst[i] = csrc[i];
    }
}
