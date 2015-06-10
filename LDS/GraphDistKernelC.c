#include<stdio.h>
#include<stdlib.h>
#include<math.h>
double ** graphDistKernelC(double **X, double **D2, double **NN,long rows,long columns,long nofNn)
{
	long dimension,i,j;
	double meanD2=0;
	dimension=(nofNn==0)?rows:nofNn;
	for(i,dimension)
	{
		for(j,columns)
		{
			meanD2+=pow(D2[i][j],0.5);
		}
	}
	meanD2=pow(meanD2/dimension*columns,2);
	for(i,dimension)
	{
		for(j,columns)
		{
			D2[i][j]=D2[i][j]/meanD2;
		}
	}

}
