#include <stdio.h>
#include <stdlib.h>
double** sorting(double ** x,double ** D,long columns,long rows){
	int i,j,k;
	double t;
	D=(double**)calloc(rows,sizeof(double*));
	for(i=0;i<rows;i++)
	{
	    D[i]=(double*)calloc(columns,sizeof(double));	
    }
    for(i=0;i<rows;i++)
    {
    	for(j=0;j<columns;j++)
    	{
    		D[i][j]=i+1;
    	}
    }
	for(k=0;k<columns;k++)
	{
		for (i = 1 ; i <= rows - 1; i++) 
		{
        j = i;
		while ( j > 0 && x[j][k] < x[j-1][k]) 
			{
		      t          = x[j][k];
		      x[j][k]   = x[j-1][k];
		      x[j-1][k] = t;
		      t          = D[j][k];
		      D[j][k]   = D[j-1][k];
		      D[j-1][k] = t;
		 
		      j--;
		    }
        }
	}
	return D;
}
