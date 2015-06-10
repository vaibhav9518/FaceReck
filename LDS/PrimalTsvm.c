function [ w, b, Yu, obj ] = primal_tsvm( X, Y, opt, w0);
% === Solve the TSVM problem in the primal

  X = [X ones(length(Y),1)]; % Append one dimension for b

  % Global variables (not avoid giving them as arguments to obj_fun)
  global X_;  X_ =X;         
  global Xu_; Xu_=X(Y==0,:)'; % To speed-up obj_fun
  global R; % Rotation matrix to enforce constraint (5)
  
  % At each step, C* will be multiplied by exponent (2 in the paper)
  exponent = (opt.Cfinal/opt.Cinit)^(1/(opt.nofIter+1));
  C = opt.C;
  C2 = opt.Cinit;

  n = size(X,2);
  
  % We want to enforce vbal'*w = cbal
  vbal = ones(1,size(X,1));
  if any(Y==0)
    vbal(Y~=0) = 0;
  end;
  vbal = vbal*X;
  cbal = mean(Y(Y~=0)) * sum(vbal);
  % That can be done by rotating w in a new basis which has as
  % first component vbal. The first component of w in the new basis
  % in then fixed to w1
  [R,foo1,foo2] = svd(vbal'); clear foo1 foo2;
void primal_tsvm(double **X, double *Y,double ** w0,long rows,long columns,long Y_rows,double Cfinal,double Cinit,long nofIter)  
{
	double ** new_X=(double**)calloc(rows,sizeof(double*));
	double **Xu,**R,C,C2,exponent;
	long i,j,zeros;
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
