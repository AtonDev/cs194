
/* Two Stage reduction Kernel. IN is the vector
 * of length LENGTH in global memory that we are going
 * to reduce. SCRATCH is the local memory needed for the
 * parallel reduction stage. OUT is the partially reduced
 * vector IN. The kernel needs to be repeated till size of
 * OUT is one float. */
__kernel void reduce(__global float* in,
		     __const int length,
		     __local float* scratch,
		     __global float* out) {
  //Sequential stage of the algorithm
  float accumulator = 0;
  int global_index = get_global_id(0);
  while (global_index < length) {
    accumulator += in[global_index];
    global_index += get_global_size(0);
  }
  int local_index = get_local_id(0);
  scratch[local_index] = accumulator;
  barrier(CLK_LOCAL_MEM_FENCE);
  //done with sequential stage of the algorithm

  //parallel stage
  for (int offset = get_local_size(0) / 2;
       offset > 0;
       offset = offset / 2) {

    if (local_index < offset) {
      scratch[local_index] += scratch[local_index + offset];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  //done with parallel stage

  //store in out
  if (local_index == 0) {
    out[get_group_id(0)] = scratch[0];
  }

}
