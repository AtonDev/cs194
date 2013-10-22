__kernel void reduce(__global int *in, __global int *out, 
		     __local int *buf, int n)
{
  size_t local_id = get_local_id(0);
  size_t group_id = get_group_id(0);
  size_t global_id = get_global_id(0);
  //semi sequential stage of the algorithm
  float accumulator = 0;
  int global_index = global_id;
  while (global_index < n) {
    accumulator += in[global_index];
    global_index += get_global_size(0);
  }
  buf[local_id] = accumulator;
  barrier(CLK_LOCAL_MEM_FENCE);
  //done with sequential stage

  //parallel stage
  for (int offset = get_local_size(0) / 2;
       offset > 0;
       offset = offset / 2) {
    if (local_id < offset) {
      buf[local_id] += buf[local_id + offset];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  //done with parallel stage of algorithm


  //store in out
  if (local_id == 0) {
    out[group_id] = buf[0];
  }
}
