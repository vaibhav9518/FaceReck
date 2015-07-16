#include<stdio.h>
#include<math.h>
#include<malloc.h>
#define FOR(i,n) for(i=0;i<n;i++)
typedef double DD;
typedef long LL;
// 0 -Gauss
// 1 -Laplace
double ** calcRbfKernel( double **D2,long rows,long columns,long rbfName,double  sigma, long symmetrize )
{
	long i ,j;
	DD** new_K;
	DD**K = (DD**)calloc(rows, sizeof(DD*));
	
	FOR(i, rows)
	{
		K[i] = (DD*)calloc(columns, sizeof(DD));
		
	}
    if(rbfName==0)
    {
        if( sigma >=pow(10,320) )
        {
                  FOR(i,rows)
                  {
  	                 FOR(j,columns)
  	                 K[i][j]=-D2[i][j];
                  }
        }
        else
        {
  	              FOR(i,rows)
                  {
  	                 FOR(j,columns)
  	                   K[i][j]=exp( (-0.5/pow(sigma,2)) * D2[i][j] );
                  }
        }
    }
    else if(rbfName==1)
	{
        FOR(i,rows)
        {
  	            FOR(j,columns)K[i][j]=exp( (-1/sigma) * pow(D2[i][j],0.5) );
        }	
    }
    else
	{
 	       printf("error");
    }
    if(symmetrize)
    {
       if(rows>columns)
       {
   	       new_K = (DD**)calloc(columns, sizeof(DD*));
	       FOR(i, columns)
	       {
			    new_K[i] = (DD*)calloc(columns, sizeof(DD));
		        FOR(j,columns)	
		        new_K[i][j]=(K[i][j]+K[j][i])/2;
	       }
       }
       else
       {
		   	FOR(i, rows)
			{
			    FOR(j,columns)	
			    K[i][j]=(K[i][j]+K[j][i])/2;
			}
       }
   }
  return K;
}
