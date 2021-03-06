/* The __kernel qualifier declares a function
 * that can be executed by an application running
 * on an OpenCL device.
 * The __global qualifier declares that the pointer
 * to Y can point only to the global memory pool.
 * i.e. Y must be in the global memory pool.*/
__kernel void incr (__global float *Y, int n)
{
  /* get_global_id(0) returns the global index of the
   * of the current work item. The 0 argument indicates
   * dimension 0. You can give dimensional indices to
   * work items. In this case it is linear. */
  int idx = get_global_id(0);
  /* If the global index of the work item is less than
   * the size of the array at Y, add 1 to the value of the
   * at adress Y + idx. */
  if(idx < n)
    {
      Y[idx] = Y[idx] + 1.0f;
    }
}
/* We are adding 1 to every value of the array at Y */
