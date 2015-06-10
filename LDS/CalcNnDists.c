#include "sq_dist.c"
#include "indices.c"
#include<malloc.h>
#include<stdio.h>
#include <math.h>
typedef struct{
   double **k;
   double **D;
}output;
output CalcNnDists(double **k,long rows,long columns,long nofNn,double **D)
{
	long i,j;
	DD  **temp;
	temp=k;
    k=sq_dist(k,rows,columns);
    if(nofNn==0)D=NULL;
    else{
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
    }}
    output t={k,D};
    return t;
}
int main(){
  long rows=3,columns=2,i,j;	
  DD  **temp,**D;
  DD** k=(DD**)calloc(rows,sizeof(DD*));
  FOR(i,rows)
  {
	k[i]=(DD*)calloc(columns,sizeof(DD));	
  }
  k[0][0]=1;k[0][1]=2;//k[0][2]=3;
  k[1][0]=1;k[1][1]=2;//k[1][2]=5;
  k[2][0]=3;k[2][1]=5;//k[2][2]=9;
  temp=k;
  output t;
  t=CalcNnDists(k,rows,columns,1,D);
  //k=sq_dist(k,rows,columns);
  FOR(i,rows)
  {
  	FOR(j,rows)
  	printf("%lf ",t.k[i][j]);
  	printf("\n");
  }
  printf("\n");
  if(t.D!=NULL)
  {
  FOR(i,rows)
  {
  	FOR(j,rows)
  	printf("%lf ",t.D[i][j]);
  	printf("\n");
  }}
  return 0;
}

