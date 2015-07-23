#include<malloc.h>
#include<stdio.h>
#include <math.h>
#define FOR(i,n) for(i=0;i<n;i++) 
typedef double DD;
double ** sq_dist(double **a,long n,long D)
{
	
  double  **C, z, t;
  long   i, j, k;
  C=(DD**)calloc(n,sizeof(DD*));
  FOR(i,n)
  {
	C[i]=(DD*)calloc(n,sizeof(DD));	
  }
  for (i=0; i<n; i++) 
  for (j=0; j<n; j++) 
	{
      z = 0.0;
      for (k=0; k<D; k++) 
	  { 
	  t = a[i][k] - a[j][k]; 
	  z += t*t; 
	  }
      C[i][j] = z;
    }
    return C;
}

