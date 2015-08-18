#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <functions.h>
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
}

