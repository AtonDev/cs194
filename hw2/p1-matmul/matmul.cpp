void matmuld(double **a, double **b, double **c)
{
  for(int i=0;i<1024;i++)
    for(int k=0;k<1024;k++)
      for(int j=0;j<1024;j++)
	c[i][j] += a[i][k]*b[k][j];
}
