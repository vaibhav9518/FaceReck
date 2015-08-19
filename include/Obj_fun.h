#define FOR(i,n) for(i=0;i<n;i++)

typedef double DD;
typedef long LL;

DD** copy(DD** matrix,long rows,long columns,double scalar)
{
	// Function to return a new matrix having values same as 'matrix'
	int i,j;
	DD** temp=(DD**)calloc(rows,sizeof(DD*));
	for(i=0;i<rows;i++)
	{
		temp[i]=(DD*)calloc(columns,sizeof(DD));
		for(j=0;j<columns;j++)
		{
			temp[i][j]=scalar*matrix[i][j];
		}
	}
	return temp;
}



typedef struct{
	DD obj;
	MAT mat;
} Output_Obj;


Output_Obj obj_fun(MAT w_in,double w1,double *Y,long Y_rows,double C,double C2,double s,MAT X,MAT Xu,MAT R)
{
	//Calculate cost function
	long i,j;
	Output_Obj O;
	MAT w, out;
	double **temp=(DD**)calloc(w_in.rows+1,sizeof(DD*));
	for(i=0;i<w_in.rows+1;i++)
	{
		temp[i]=(DD*)calloc(1,sizeof(DD));
		if(i!=0)temp[i][0]=w_in.matrix[i-1][0];
		else temp[i][0]=w1;
	}
	w.matrix = Multiply_Matrices(R.matrix, temp, R.rows, 1, w_in.rows + 1); w.rows = R.rows; w.columns = 1;
	FOR(i, w_in.rows)free(temp[i]);
	free(temp);
	out.matrix = Multiply_Matrices(X.matrix, w.matrix, X.rows, w.columns, R.rows); out.rows = X.rows; out.columns = w.columns;
    long zeros=0;
    for(i=0;i<Y_rows;i++)
    {
    	if(Y[i]==0)zeros++;
    }
    double* lab=(DD*)calloc(Y_rows-zeros,sizeof(DD));
    double* unlab=(DD*)calloc(zeros,sizeof(DD));
    zeros=0;
    for(i=0;i<Y_rows;i++)
    {
    	if(Y[i]==0)
    	{
    		unlab[zeros++]=i+1;
    	}
    	else{
    		lab[i-zeros]=i+1;
    	}
    }
    double zeros2=zeros;
    zeros=0;
    for(i=0;i<Y_rows;i++)
    {
    	if((out.matrix[i][0]*Y[i]<1)&&(Y[i]!=0))zeros+=1;
    }
    double *sv=(DD*)calloc(zeros,sizeof(DD));
    zeros=0;
    for(i=0;i<Y_rows;i++)
    {
    	if((out.matrix[i][0]*Y[i]<1)&&(Y[i]!=0)){
    		sv[zeros++]=i+1;
    	}
    }
    double **cost_unlab=(DD**)calloc(zeros2,sizeof(DD*));
    double sum_new=0;
    for(i=0;i<zeros2;i++)
    {
    	cost_unlab[i]=(DD*)calloc(1,sizeof(DD));
    	cost_unlab[i][0]=exp(-s*pow(out.matrix[(int)unlab[i]-1][0],2));
    	sum_new+=cost_unlab[i][0];
    }
    double **w0=copy(w.matrix,w.rows,w.columns,1);
    w0[w.rows-1][w.columns-1]=0;
    double **obj1=Multiply_Matrices(Transpose(w0,w.rows,w.columns),w0,w.columns,w.columns,w.rows);
    double obj=0.5*obj1[0][0];
	
    obj+=C2*sum_new;
	sum_new = 0;
    for(i=0;i<zeros;i++)
    {
		sum_new += pow(1 - (out.matrix[(int)sv[i] - 1][0] * Y[(int)sv[i] - 1]), 2);
    }
    obj+=C*sum_new;
    double **grad=copy(w0,w.rows,w.columns,1);
    double ** temp1=(DD**)calloc(zeros,sizeof(DD*));
    double ** temp2=(DD**)calloc(zeros,sizeof(DD*));
    for(i=0;i<zeros;i++)
    {
    	temp1[i]=(DD*)calloc(1,sizeof(DD));
		temp1[i][0] = out.matrix[(int)sv[i] - 1][0] - Y[(int)sv[i] - 1];
    	temp2[i]=(DD*)calloc(X.columns,sizeof(DD));
    	for(j=0;j<X.columns;j++)
    	temp2[i][j]=X.matrix[(int)sv[i]-1][j];
    }
    temp2=Multiply_Matrices(Transpose(temp2,zeros,X.columns),temp1,X.columns,1,zeros);
    for(i=0;i<w.rows;i++)
    {
    	for(j=0;j<w.columns;j++)
    	{
    		grad[i][j]+=2*C*temp2[i][j];
    	}
    }
    
    for(i=0;i<zeros;i++)
    {
    	temp1[i][0]=2*C2*s*cost_unlab[i][0]*out.matrix[(int)unlab[i]-1][0];
    }
    FOR(i,X.columns)free(temp2[i]);free(temp2);
    temp2=Multiply_Matrices(Xu.matrix,temp1,Xu.rows,1,zeros);
    for(i=0;i<w.rows;i++)
    {
    	for(j=0;j<w.columns;j++)
    	{
    		grad[i][j]-=temp2[i][j];
    	}
    }
    FOR(i,Xu.rows)free(temp2[i]);free(temp2);
    temp2=Transpose(grad,w.rows,w.columns);
    grad=Multiply_Matrices(temp2,R.matrix,w.columns,R.columns,w.rows);
    FOR(i,w.columns)free(temp2[i]);free(temp2);
    temp2=(DD**)calloc(w.columns*R.columns-1,sizeof(DD*));
    zeros=0;
    for(i=0;i<w.columns;i++)
    {
    	for(j=0;j<R.columns;j++)
    	{
    		if(i!=0||j!=0)
			{
    		temp2[zeros]=(DD*)calloc(1,sizeof(DD));
			temp2[zeros++][0]=grad[i][j];
    	    }
    	}
    }
    FOR(i,w.columns)free(grad[i]);free(grad);
    free(lab);free(unlab);free(sv);
    MAT matr;
	matr.matrix=temp2;
	matr.rows=w.columns*R.columns-1;matr.columns=1;
	O.obj=obj;
	O.mat=matr;
	return O;
}

