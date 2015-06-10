
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
/*
* INPUT: A Matrix Mat1 of Size MxP
*		: A Matrix Mat2 of Size PxQ
*		: The Dimensions M, Q, P
* OUTPUT: Resultant Matrix
*/
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

void primal_tsvm(double **X, double *Y,double ** w0,long w0_rows,long w0_columns,long rows,long columns,long Y_rows,double Cfinal,double C,double Cinit,long nofIter)  
{
	double ** new_X=(double**)calloc(rows,sizeof(double*));
	double **Xu,**R,C1,C2,exponent,**vbal,cbal=0,sum_vbal=0;
	vbal=(double**)calloc(1,sizeof(double*));
	vbal[0]=(double*)calloc(rows,sizeof(double));
	long i,j,zeros,n;
	for(i=0;i<rows;i++)vbal[0][i]=1;
	for(i=0;i<rows;i++)
	{
	new_X[i]=(double*)calloc(columns+1,sizeof(double));
	FOR(j,columns)new_X[i][j]=X[i][j];
	new_X[i][columns]=1;
    }
    for(i=0;i<Y_rows;i++){if(Y[i]==0)zeros++;}
    Xu=(double**)calloc(zeros,sizeof(double*));
    for(i=0;i<zeros;i++)
	{
	Xu[i]=(double*)calloc(columns+1,sizeof(double));
    }
    zeros=0;
    for(i=0;i<Y_rows;i++)
    {
    	if(Y[i]==0)
    	{
    		for(j=0;j<columns+1;j++)
    		{
    			Xu[zeros][j]=X[i][j];
    		}
    		zeros++;
    	}
    }
    exponent=pow(Cfinal/Cinit,1/(nofIter+1));
    C1=C;
    C2=Cinit;
    n=columns+1;
    if(zeros>0)
    {
    	for(i=0;i<Y_rows;i++)
    	{
    		if(Y_rows[i]>0)
    		{
    			vbal[0][i]=0;
    		}
    	}
    }
    vbal=Multiply_Matrices(vbal,new_X,1,rows,columns+1);
    j=0;
    for(i=0;i<Y_rows;i++)
    	{
    		if(Y_rows[i]>0)
    		{
    			j+=1
    			cbal+=Y_rows;
    		}
    	}
    for(i=0;i<columns+1;i++)sum_vbal+=vabl[0][i];
    cbal=(cbal/j)*sum_vbal;
	vbal=Transpose(vbal,1,columns+1);
	SVD(vbal,columns+1,1,R);
	DD** newR=(double**)calloc(columns+1,sizeof(double*));
	for(i=0;i<columns+1;i++)
	{
		newR[i]=(double*)calloc(1,sizeof(double));
		newR[i][0]=R[i][0];
	}
	DD** w=Multiply_Matrices(vbal,newR,1,1,columsn+1)
	DD w1=cbal/w[0][0];
	if(w0==NULL)
	{
	  DD** w=(double**)calloc(n-1,sizeof(double*));	
	  for(i=0;i<n-1;i++)
	  {
	  	w[i]=(double*)calloc(1,sizeof(double));	
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
}
  /*
  w1 = cbal/(vbal*R(:,1));
  
  if nargin<4    % If w0 is given, start with it.
    w = zeros(n-1,1);
  else
    w = R(:,2:end)'*w0;
  end;
  
  % Check if the optimization toolbox if available
  opt_tb = opt.opt_tb & license('checkout','Optimization_toolbox');
  
  % Initialize the options for the optimizer
  maxiter = opt.maxiter*size(X,1);       % Maxinum number of iterations
  tolfun = opt.tolfun*C;                 % Stopping criterion
  if opt_tb
    opt2 = optimset('GradObj','on','LargeScale','off',...
                    'Display','off','TolFun',tolfun,'MaxIter',maxiter);
    if opt.verb >= 3
      opt2 = optimset(opt2,'Display','iter');
    end;
  else
    opt2.length = maxiter;
    opt2.tolX = 0; 
    opt2.tolFun = tolfun;
    opt2.verb = opt.verb - 1;
  end; 
  
  % Main loop
  for i = 0:opt.nofIter
    C2 = C2*exponent;
    if opt.verb == 1, fprintf('.'); end;
    
    if opt_tb
      [w,obj,flag,out,grad] = fminunc(@obj_fun,w,opt2,w1,Y,C,C2,opt.s);
      iter = out.iterations;
    else
      [w,fX,count] = minimize(w,@obj_fun,opt2,w1,Y,C,C2,opt.s);
      iter = length(fX);
      [obj, grad] = obj_fun(w,w1,Y,C,C2,opt.s);
      flag = (iter < opt2.length);
    end;

    if opt.verb >= 2
      fprintf(['  Iter = %d\t C* = %f\t Obj = %f [%d iterations, ' ...
               'norm of grad = %f]\n'],i,C2,obj,iter,norm(grad));
    end;
 
    if ~flag & (opt.verb >= 1)
       warning('Maximum number of iterations reached');
    end;
  end;
  
  % Rotate back w and get w and b
  w = R*[w1; w];  
  Yu = Xu_'*w;
  b = w( n );
  w = w( 1:n-1 );

   if opt.verb == 1, fprintf('\n'); end;
   if opt.verb >= 1, fprintf('Done !\n'); end;*/
