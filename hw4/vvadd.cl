__kernel void vvadd (__global float *Y, __global float *A, __global float *B,
	 int n)
{
  /* CS194: implement the body of this kernel */
  /* Let every work item do an addition. */
  int id = get_global_id(0);
  if (id < n)
     Y[id] = A[id] + B[id];
}
