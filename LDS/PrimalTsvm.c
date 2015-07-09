
DD** Transpose(DD **Mat1, LL Rows, LL Columns)
{
	LL i, j;
	DD** Temp = (DD**)calloc(Columns, sizeof(DD*));
	for (i = 0; i<Columns; i++)
	{
		Temp[i] = (DD*)calloc(Rows, sizeof(DD));
	}
	for (i = 0; i<Rows; i++)
	{
		for (j = 0; j<Columns; j++)
		{
			Temp[j][i] = Mat1[i][j];
		}
	}
	return Temp;
}
DD** Multiply_Matrices(DD **Mat1, DD **Mat2, LL M, LL Q, LL P)
{
	LL i, c, d, k;
	DD Sum = 0;
	//Mat1=Precise(Mat1,M,P);
	//Mat2=Precise(Mat2,P,Q);
	DD** Temp = (DD**)calloc(M, sizeof(DD*));
	FOR(i, M)
		Temp[i] = (DD*)calloc(Q, sizeof(DD));

	FOR(c, M)
	{
		FOR(d, Q)
		{
			FOR(k, P)
			{
				Sum += (Mat1[c][k] * Mat2[k][d]);
			}
			Temp[c][d] = Sum;
			Sum = 0;
		}
	}
	return Temp;
}
typedef struct{
    DD delta;       
    LL nofIter;        
    DD C;         
    DD Cinit;      
    DD Cfinal;         
    LL nofNn;         
    DD sigma;       
    DD opt_tb;         
    LL maxiter;         
    DD tolfun;      
    DD s;
    DD* splits;
} Opt;
typedef struct{
    DD length;
    DD tolX; 
    DD tolFun;
} Opt2;
typedef struct{
	LL rows;
	LL columns;
	DD** matrix;
} MAT;
void primal_tsvm(MAT X, double *Y,MAT w0,long Y_rows,Opt opt)  
{
	MAT new_X;
	new_X.matrix=(double**)calloc(X.rows,sizeof(double*));
	new_X.rows=X.rows;new_X.columns=X.columns+1;
	double **Xu,**R,C1,C2,exponent,**vbal,cbal=0,sum_vbal=0,tolfun2;
	MAT Xu,R,vbal;
	vbal.matrix=(double**)calloc(1,sizeof(double*));vbal.rows=1;vbal.columns=X.rows;
	vbal[0]=(double*)calloc(vbal.columns,sizeof(double));
	long i,j,zeros,n,maxiter2;
	for(i=0;i<new_X.rows;i++)
	{
    vbal.matrix[0][i]=1;		
	new_X.matrix[i]=(double*)calloc(new_X.columns,sizeof(double));
	FOR(j,columns)new_X.matrix[i][j]=X.matrix[i][j];
	new_X.matrix[i][columns]=1;
    }
    for(i=0;i<Y_rows;i++){if(Y[i]==0)zeros++;}
    Xu.matrix=(double**)calloc(zeros,sizeof(double*));
    Xu.rows=zeros;
    Xu.columns=X.columns+1;
    for(i=0;i<Xu.rows;i++)
	{
	Xu.matrix[i]=(double*)calloc(Xu.columns,sizeof(double));
    }
    zeros=0;
    for(i=0;i<Y_rows;i++)
    {
    	if(Y[i]==0)
    	{
    		for(j=0;j<Xu.columns;j++)
    		{
    			Xu.matrix[zeros][j]=new_X.matrix[i][j];
    		}
    		zeros++;
    	}
    }
    Xu=Transpose(Xu,Xu.rows,Xu.columns);i=Xu.rows;Xu.rows=Xu.columns;Xu.columns=i;
    exponent=pow(opt.Cfinal/opt.Cinit,1/(opt.nofIter+1));
    C=opt.C;
    C2=opt.Cinit;
    n=columns+1;
    if(zeros>0)
    {
    	for(i=0;i<Y_rows;i++)
    	{
    		if(Y_rows[i]!=0)
    		{
    			vbal.matrix[0][i]=0;
    		}
    	}
    }
    vbal.matrix=Multiply_Matrices(vbal,new_X,vbal.rows,new_X.columns,vbal.columns);vbal.columns=new_X.columns;
    j=0;
    for(i=0;i<Y_rows;i++)
    	{
    		if(Y_rows[i]!=0)
    		{
    			j+=1
    			cbal+=Y_rows[i];
    		}
    	}
    for(i=0;i<columns+1;i++)sum_vbal+=vabl[0][i];
    cbal=(cbal/j)*sum_vbal;
	SVD(Transpose(vbal,1,columns+1);,columns+1,1,R);
	DD** newR=(double**)calloc(columns+1,sizeof(double*));
	for(i=0;i<columns+1;i++)
	{
		newR[i]=(double*)calloc(1,sizeof(double));
		newR[i][0]=R[i][0];
	}
	DD** w=Multiply_Matrices(vbal,newR,1,columns+1,1)
	DD w1=cbal/w[0][0];
	if(w0==NULL)
	{
	  DD** w=(double**)calloc(n-1,sizeof(double*));	
	  for(i=0;i<n-1;i++)
	  {
	  	w[i]=(double*)calloc(1,sizeof(double));
		for(j=0;j<n-1;j++)
	  }
	}
	else{
		DD** new_newR=(double**)calloc(columns+1,sizeof(double*));	
		for(i=0;i<columns+1;i++)
		{
			new_newR[i]=(double*)calloc(columns,sizeof(double));	
			for(j=0;j<columns;j++)
			{
				new_newR[i][j]=R[i][j+1];
			}
		}
		DD** w=Multiply_Matrices(Transpose(new_newR,columns+1,columns),w0,columns,w0_rows,columns+1);
	}
	maxiter2=opt.maxiter*rows;
	tolfun2=opt.tolfun*C;
	Opt2 opt2;
	opt2.length = maxiter;
    opt2.tolX = 0; 
    opt2.tolFun = tolfun;
    for(i=0;i<opt.nofIter+1;i++)
    {
    	minimize(w,@obj_fun,opt2,w1,Y,C,C2,opt.s);
    }
	
}
  
