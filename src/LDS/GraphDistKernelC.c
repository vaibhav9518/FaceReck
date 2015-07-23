#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "rhoPathDists2.cpp"
MAT graphDistKernelC(MAT X,MAT D2, MAT NN,Param param,double * idx,long rows3)
{
	long dimension,i,j;
	double meanD2=0;
	if(param.nofNn==0)
	{
		if(D2.rows!=D2.columns)error("error in GraphDistKernelC\n");
	}
	else
	{
		if(D2.rows!=param.nofNn)error("error in GraphDistKernelC\n");
	}
	FOR(i,D2.rows)
	{
		FOR(j,D2.columns)
		{
			meanD2+=pow(D2.matrix[i][j],0.5);
		}
	}
	meanD2=pow(meanD2/(D2.rows*D2.columns),2);
	FOR(i,D2.rows)
	{
		FOR(j,D2.columns)
		{
			D2.matrix[i][j]=D2.matrix[i][j]/meanD2;
		}
	}
	MAT E2=final(D2,NN,idx,rows3,param.pathNorm);
	if(isinf(param.sigma))
	{
        //printf("\nis inf");
		return E2;
	}   
}


