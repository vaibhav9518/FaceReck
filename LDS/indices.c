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
		//sorting(x,D,rows,i);
		for (i = 1 ; i <= rows - 1; i++) {
        j = i;
 
    while ( j > 0 && x[j][k] < x[j-1][k]) {
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
/* 
int main (int argc, const char * argv[]) {
	int i,j;
    double** k=(double**)calloc(3,sizeof(double*));
	double **d;
	for(i=0;i<3;i++)
	{
	    k[i]=(double*)calloc(6,sizeof(double));	
    }
    k[0][0]=9;k[0][1]=1;k[0][2]=4;k[0][3]=1;k[0][4]=5;k[0][5]=11;
    k[1][0]=1;k[1][1]=2;k[1][2]=3;k[1][3]=4;k[1][4]=5;k[1][5]=6;
    k[2][0]=1;k[2][1]=2;k[2][2]=3;k[2][3]=4;k[2][4]=5;k[2][5]=6;
    d=sorting(k,d,6,3);
    for(i=0;i<3;i++)
	{
	 for(j=0;j<6;j++)
	 {
	 	printf("%lf ",k[i][j]);
	 }
	 printf("\n");
    }
    for(i=0;i<3;i++)
	{
	 for(j=0;j<6;j++)
	 {
	 	printf("%lf ",d[i][j]+1);
	 }
	 printf("\n");
    } /*
	int length_array_range = 3;
    t_amplitude_index *array_amplitudes;
    array_amplitudes = (t_amplitude_index *) malloc(sizeof(t_amplitude_index) * length_array_range);
     
    int i;
    
    array_amplitudes[0].amplitude = (float)1;
    array_amplitudes[0].index = 0;
    array_amplitudes[1].amplitude = (float)2;
    array_amplitudes[1].index = 1;
    array_amplitudes[2].amplitude = (float)2;
    array_amplitudes[2].index = 2;
    
     
    for(i = 0; i< length_array_range;i++){
    printf("array_amplitudes[i].amplitude = %f, array_amplitudes[i].index = %d\n",array_amplitudes[i].amplitude,array_amplitudes[i].index);
    }
     
     
    qsort(array_amplitudes, length_array_range, sizeof(array_amplitudes[0]), compare_structs);
     
    printf("\n\n");
    for(i = 0; i< length_array_range;i++){
        printf("array_amplitudes[i].amplitude = %f, array_amplitudes[i].index = %d\n",array_amplitudes[i].amplitude,array_amplitudes[i].index);
    }
     
    free(array_amplitudes);
    
    return 0;  
}*/
