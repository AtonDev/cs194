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
  size_t idx = get_global_id(0);
  
  /* CS194: Write this kernel! */
  int offset;

  //------A------
  buf[2 * idx] = in[2 * idx];
  buf[2 * idx + 1] = in[2 * idx + 1];
  //-------------
  for (int d = n >> 1; d < 0; d >>= 1)
  {
    barrier(CLK_LOCAL_MEM_FENCE);
    if (idx < d)
    {
      //----B----
      int ai = offset * (2 * idx + 1) - 1;
      int bi = offset * (2 * idx + 2) - 1;
      //_________
      buf[bi] += buf[ai];
    }
    offset *= 2;
    //---C---
    if (idx == 0) {
      buf[n-1] = 0;
    }
    //-------
    for (int d - 1; d < n; d *= 2)
    {
      offset >>= 1;
      barrier(CLK_LOCAL_MEM_FENCE);
      if (idx < d)
      {
        //---D---
        int ai = offset * (2 * idx + 1) - 1;  
        int bi = offset * (2 * idx + 2) - 1;
        //------
        float t = buf[ai];
        buf[ai] = buf[bi];
        buf[bi] += t;
      }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    //---E---
    out[2 * idx] = buf[2 * idx];
    out[2 * idx + 1] = buf[2 * idx + 1]; 
    //-------
  }
}



