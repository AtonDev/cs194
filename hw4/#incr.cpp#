#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "clhelp.h"

int main(int argc, char *argv[])
{
  std::string incr_kernel_str;

  /* Provide names of the OpenCL kernels
   * and cl file that they're kept in */
  std::string incr_name_str =
    std::string("incr");
  std::string incr_kernel_file =
    std::string("incr.cl");


  cl_vars_t cv;
  cl_kernel incr;

  /* Read OpenCL file into STL string */
  readFile(incr_kernel_file,
	   incr_kernel_str);

  /* Initialize the OpenCL runtime
   * Source in clhelp.cpp */
  initialize_ocl(cv);

  /* Compile all OpenCL kernels */
  compile_ocl_program(incr, cv, incr_kernel_str.c_str(),
		      incr_name_str.c_str());

  /* Arrays on the host (CPU) */
  float *h_Y, *h_YY;
  /* Arrays on the device (GPU) */
  cl_mem g_Y;

  int n = (1<<20);
  h_Y = new float[n];
  h_YY = new float[n];

  for(int i = 0; i < n; i++)
    {
      h_YY[i] = h_Y[i] = (float)drand48();
    }

  cl_int err = CL_SUCCESS;
  /* CS194: Allocate memory for arrays on
   * the GPU */
  /* Creates a buffer in the cv.context context, with read and write access
   * at the global host adress g_Y, of size sizeof(float)*n. */
  g_Y = clCreateBuffer(cv.context,CL_MEM_READ_WRITE,sizeof(float)*n,NULL,&err);
  CHK_ERR(err);

  /* enqueue commands to write to the buffer g_Y from hos memory.
   * Commands will be queued in cv.commands.
   * true indicates that the write is put on the commands queue.
   * 0 is the offset in bytes in the buffer object to write to.
   * sizeof(float)*n is the size in byte of data being wirtten.
   * h_Y is the address in host memory of the data being written from.
   */
   err = clEnqueueWriteBuffer(cv.commands, g_Y, true, 0, sizeof(float)*n,
			     h_Y, 0, NULL, NULL);
   /* checks whether the write buffer command was succesful. */
  CHK_ERR(err);

  /* declaring the global size of th y dimension to be n. */
  size_t global_work_size[1] = {n};
  /* declaring the size of work groups to be 128 work items. */
  size_t local_work_size[1] = {128};

  /* Sets specific arguments for the kernel incr.
   * 0 is the argument index, sizeof(cl_mem) is the size
   * of the argument, which is the pointer to g_Y.*/
  err = clSetKernelArg(incr, 0, sizeof(cl_mem), &g_Y);
  CHK_ERR(err);

  /* Sets specific arguments for the kernel incr.
   * 1 is the argument index, sizeof(int) is the size
   * of the argument, which is the pointer to n.*/
  err = clSetKernelArg(incr, 1, sizeof(int), &n);
  CHK_ERR(err);

  /* Enqueues a command on cv.commands to execute the
   * kernel incr.cl on the device. Uses linear dimension
   * to specify work groups and items and specifies to use
   * global_work_size work items for the execution and local_work_size
   * as the size of a work group.  */
  err = clEnqueueNDRangeKernel(cv.commands,
			       incr,
			       1,//work_dim,
			       NULL, //global_work_offset
			       global_work_size, //global_work_size
			       local_work_size, //local_work_size
			       0, //num_events_in_wait_list
			       NULL, //event_wait_list
			       NULL //
			       );
  CHK_ERR(err);

  /* Read result of GPU on host CPU */
  err = clEnqueueReadBuffer(cv.commands, g_Y, true, 0, sizeof(float)*n,
			    h_Y, 0, NULL, NULL);
  CHK_ERR(err);

  /* Check answer */
  bool er = false;
  for(int i = 0; i < n; i++)
    {
      float d = (h_YY[i] + 1.0f);
      if(h_Y[i] != d)
	{
	  printf("error at %d :(\n", i);
	  er = true;
	  break;
	}
    }
  if(!er)
    {
      printf("CPU and GPU results match\n");
    }

  uninitialize_ocl(cv);

  delete [] h_Y;
  delete [] h_YY;

  clReleaseMemObject(g_Y);

  return 0;
}
