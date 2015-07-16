#include<stdio.h>
#include<malloc.h>
#include<math.h>
void quicksort(double* x,long first,long last){
    long pivot,j,i;
    double temp;
     if(first<last){
         pivot=first;
         i=first;
         j=last;
         while(i<j){
             while(x[i]<=x[pivot]&&i<last)
                 i++;
             while(x[j]>x[pivot])
                 j--;
             if(i<j){
                 temp=x[i];
                 x[i]=x[j];
                 x[j]=temp;
             }
         }
         temp=x[pivot];
         x[pivot]=x[j];
         x[j]=temp;
         quicksort(x,first,j-1);
         quicksort(x,j+1,last);

    }
}
/*function [ sigma ] = calcDefaultSigma( e2, k );

e2 = sort( e2 );
nl = length( e2 );
sigma2 = e2( 1 + round( nl / k ) );
if( sigma2 == +inf )
  idx = find( e2 < inf );
  if( length(idx) == 0 )
    sigma2 = 1;
  else
    sigma2 = max( e2(idx) );
    if( sigma2 <= 0 )
      sigma2 = 1;
    end;
  end;
end;
if( sigma2 <= 0 )
  idx = find( e2 > 0 );
  if( length(idx) == 0 )
    sigma2 = 1;
  else
    sigma2 = min( e2(idx) );
    if( sigma2 == +inf )
      sigma2 = 1;
    end;
  end;
end;
assert( 0 < sigma2 & sigma2 < +inf );
sigma = sqrt( sigma2 );*/
double calcDefaultSigma(double *e2,double k,long size)
{
	double sigma2;
	long i;
	quicksort(e2,0,size-1);
	sigma2=e2[((int)round(size/k))];
	if(sigma2>pow(10,320))
	{
		for(i=0;i<size;i++)
		{
			if(e2[i]<pow(10,320))
			{
				if(sigma2>pow(10,320))sigma2=e2[i];
				else sigma2=(sigma2>e2[i])?sigma2:e2[i];
			}
		}
		if((sigma2>pow(10,320))|(sigma2<=0))sigma2=1;
	}
	if(sigma2<=0)
	{
		for(i=0;i<size;i++)
		{
			if(e2[i]>0)
			{
				if(sigma2<=0)sigma2=e2[i];
				else sigma2=(sigma2<e2[i])?sigma2:e2[i];
			}
		}
		if((sigma2<=0)|(sigma2>pow(10,320)))sigma2=1;
	}
	return pow(sigma2,0.5);
}/*
int main(){
	int i;
	double *x=(double*)calloc(3,sizeof(double));
	//printf("%lf \n",round(1.0/3.0));
	x[0]=3;x[1]=2;x[2]=1;
	quicksort(x,0,2);
	for(i=0;i<3;i++)printf("%lf ",x[i]);
	printf("%lf ",calcDefaultSigma(x,2,3));
	return 0;
}*/
