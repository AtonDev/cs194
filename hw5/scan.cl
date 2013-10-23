__kernel void update(__global int *in,
		     __global int *block,
		     int n)
{
  size_t idx = get_global_id(0);
  size_t tid = get_local_id(0);
  size_t dim = get_local_size(0);
  size_t gid = get_group_id(0);

  if(idx < n && gid > 0)
    {
      in[idx] = in[idx] + block[gid-1];
    }
}

__kernel void scan(__global int *in, 
		   __global int *out, 
		   __global int *bout,
		   __local int *buf,
		   int n)
{
  size_t idx = get_global_id(0);
  size_t tid = get_local_id(0);
  size_t dim = get_local_size(0);
  size_t gid = get_group_id(0);
  
  /* CS194: Write this kernel! */
  __local int buffer2[128];
  buf[tid] = in[idx];
  barrier(CLK_LOCAL_MEM_FENCE);
  for (int offset = 1; offset < dim; offset *= 2) {
    if (tid >= offset) {
       buffer2[tid - offset] = buf[tid - offset];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    if (tid >= offset) {
      buf[tid] = buffer2[tid - offset] + buf[tid];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  out[idx] = buf[tid];
  if (tid == dim - 1) {
    bout[gid] = buf[tid];
  }
}
