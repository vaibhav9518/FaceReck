/* sq_dist - a mex function to compute a matrix of all pairwise squared
   distances between two sets of vectors, stored in the columns of the two 
   matrices that are arguments to the function. The length of the vectors must
   agree. If only a single argument is given, the missing argument is taken to
   be identical to the first. If an optional third matrix argument Q is given,
   it must be of the same size as the output, but in this case a vector of the
   traces of the product of Q and the coordinatewise squared distances is
   returned.

   Copyright (c) 2003, 2004 Carl Edward Rasmussen. 2003-03-08. */
 
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

