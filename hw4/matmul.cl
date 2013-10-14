__kernel void matmul(__global float *Y, __global float *A, __global float *B, 
	 int n)
{
  /* CS194: Implement the body of this kernel */
  /* assign each 2d position in the result matrix to each work item. */
  int x = get_global_id(0);
  int y = get_global_id(1);
  float val = 0;
  /* Compute dot product to put the posisiotn (x, y). */
  for (int k = 0; k < n; k++) 
      val += A[y * n + k] * B[k * n + x];
  Y[y * n + x] = val;
}
