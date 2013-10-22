#define NUM_BANKS 16  
#define LOG_NUM_BANKS 4  
#define CONFLICT_FREE_OFFSET(n) \
  ((n) >> NUM_BANKS + (n) >> (2 * LOG_NUM_BANKS)) 

__kernel void update(__global int *in,
		     __global int *block,
		     int n)
{
  size_t idx = get_global_id(0);
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
  int idx = get_global_id(0);
  int pout = 0, pin = 1;
  buf[pout * n + idx] = (idx > 0) ? out[idx - 1] : 0;
  barrier(CLK_LOCAL_MEM_FENCE);
  for (int offset = 1; offset < n; offset *= 2)
  {
    pout = 1 - pout;
    pin = 1 - pout;
    if (idx >= offset)
      buf[pout * n + idx] += buf[pin * n + idx - offset];
    else
      buf[pout * n + idx] = buf[pin * n + idx];
    barrier(CLK_LOCAL_MEM_FENCE);
  }
  out[idx] = buf[pout * n + idx];


}



