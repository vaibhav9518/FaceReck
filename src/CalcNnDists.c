#include <sq_dist.h>
#include <indices.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
//structure for output of CalcNnDists
typedef struct{
   MAT D2;
   MAT NN;
}output;
void print(DD** mat,long rows,long columns)
{
	long i,j;
	FOR(i,rows)
	{
		FOR(j,columns)
		{
			if(isinf(mat[i][j]))printf("inf ");
			else printf("%e ",mat[i][j]);
		}
		printf("\n");
	}
}
// rows are vectors
output CalcNnDists(double **k,long rows,long columns,long nofNn)
{
	//Calculate nearest neighbour graph
	long i,j;
	DD  **temp,**D=NULL;
	temp=k;
	output t;
    k=sq_dist(k,rows,columns);// call sq_dist function to find square distances between columns
    if(nofNn==0)
	{
	// if number of neighbours is zero	   
		    D=NULL;
		    MAT x,y;
		    x.matrix=k;x.rows=rows;x.columns=rows;
		    y.matrix=D;y.rows=0;y.columns=0;
		    t.D2=x;t.NN=y;
    }
    else
	{
		    D=sorting(k,D,rows,rows);
		    FOR(i,rows)
		    {  
		   	  if(D[0][i]!=i+1)
		  	  { 
		  		FOR(j,rows)
		  		{
		  		if(D[j][i]==i+1)	
		  		{
		  			D[j][i]=D[0][i];
		  			D[0][i]=i+1;
		  		}
		  		}
		  	  } 
		    }
		    temp=k;
		    k=(DD**)calloc(nofNn,sizeof(DD*));
		    DD** temp2=D;
		    D=(DD**)calloc(nofNn,sizeof(DD*));
		    FOR(i,nofNn)
		    {
		    	k[i]=(DD*)calloc(rows,sizeof(DD));
		    	D[i]=(DD*)calloc(rows,sizeof(DD));
		    	FOR(j,rows)
		    	{
		    		k[i][j]=temp[i+1][j];
		    		D[i][j]=temp2[i+1][j];
		    	}
		    }
		    // free extra memory
		    for(i=0;i<rows;i++)
		    {
		    	free(temp[i]);
		    	free(temp2[i]);
		    }
		    MAT x,y;
		    x.matrix=k;x.rows=nofNn;x.columns=rows;
		    y.matrix=D;y.rows=nofNn;y.columns=rows;
		    t.D2=x;t.NN=y;
    }
    return t;
}


