#include <stdio.h>
#include <fstream>
#include <cmath>
#include <malloc.h>
#include <armadillo>
#include <stdlib.h>
#include <stdbool.h>
#define FOR(i,n) for(i=0;i<n;i++)
#define error(i) printf(i); 
using namespace arma;
using namespace std;
typedef double DD;
typedef long LL;
typedef struct{
	LL rows;
	LL columns;
	DD** matrix;
} MAT;
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
	DD tolX;
	LL length;
	DD tolFun;
	
} Opt2;
typedef struct{
    DD pathNorm;
    LL nofNn;
    DD annEps;
    LL rbf;
    DD sigma;
} Param;
#include "CalcNnDists.c"
#include "GraphDistKernelC.c"
#include "calcDefaultSigma.c"
#include "MDS.c"
#include "CalcRbfKernel.c"

typedef struct{
	DD obj;
	DD b;
	MAT Yu;
	MAT w;
} Output_Primal;

DD** Multiply_Matrices(DD **Mat1, DD **Mat2, LL M, LL Q, LL P)
{
	LL i, c, d, k;
	DD Sum = 0;
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
#include "minimize.c"
Output_Primal primal_tsvm(MAT X, MAT Y, MAT w0, Opt opt);
double ** train_one_split(MAT Xnldr, double *Yl, long Yl_rows, long m0, Opt opt, long classes);

DD** LDS(DD **Xl, LL d0, LL m0, DD **Xu, LL d1, LL m1, DD *Yl, LL Yl_rows, LL classes, DD rho, Opt opt)
{
	LL i, j, k, K;
	Param param;
	MAT D2, NN;
	opt.delta = 0.1;
	opt.nofIter = 10;
	opt.C = 0.390;
	opt.Cinit = 0.01;
	opt.Cfinal = 1;
	opt.nofNn = 62;
	opt.sigma = INFINITY;
	opt.opt_tb = 1;
	opt.maxiter = 3;
	opt.tolfun = exp(-5);
//	if ((d0 != d1) | ((m0 != Yl_rows) & ((m0 + m1) != Yl_rows)))error("Dimensions mismatch\n");
	if((1>opt.nofIter)|(opt.C<0)|((opt.Cinit<0)&&(opt.Cinit>opt.Cfinal))|(opt.sigma<0))error("Assertion error");
	LL m = m0 + m1;
	if((opt.nofNn<0)|(opt.nofNn>m))error("Assertion error");
	DD** mat = (DD**)calloc(m, sizeof(DD*));
	FOR(i, m)mat[i] = (DD*)calloc(d0, sizeof(DD));
	MAT X;
	X.matrix = mat; X.rows = m; X.columns = d0;
	FOR(i, X.rows)
	{
		FOR(j, X.columns)
		{
			if (i < m0)
			{
				X.matrix[i][j] = Xl[j][i];
			}
			else{
				X.matrix[i][j] = Xu[j][i - m0];
			}
		}
	}
	K = classes;
	output t = CalcNnDists(X.matrix, X.rows, X.columns, opt.nofNn);
	D2 = t.D2; NN = t.NN;
	param.pathNorm = rho;
	param.sigma = INFINITY;
	param.nofNn = opt.nofNn;
	DD* idx = (DD*)calloc(m, sizeof(DD));
	FOR(i, m)
	{
		idx[i] = i + 1;
		//printf("%lf ", idx[i]);
	}
	//if(isinf(param.sigma))printf("\n%lf not inf\n",param.sigma);
	MAT E2 = graphDistKernelC(X, D2, NN, param, idx, m);
    FOR(i,X.rows)free(X.matrix[i]);free(X.matrix); 
    FOR(i,opt.nofNn){free(D2.matrix[i]);free(NN.matrix[i]);}free(D2.matrix);free(NN.matrix);
    D2.rows=0;D2.columns=0;NN.rows=0;NN.columns=0; 
	if (opt.nofNn != 0)
	{
		DD** new_E2 = (DD**)calloc(E2.rows, sizeof(DD*));
		DD max1 = E2.matrix[0][0];
		FOR(i, E2.rows)
		{
			new_E2[i] = (DD*)calloc(E2.columns, sizeof(DD));
			FOR(j, E2.columns)
			{
				new_E2[i][j] = min(E2.matrix[i][j], E2.matrix[j][i]);
				if ((new_E2[i][j] > max1) && (!isinf(new_E2[i][j])))
				{
					max1 = new_E2[i][j];
				}
			}
		}
		FOR(i, E2.rows)free(E2.matrix[i]);free(E2.matrix);
		E2.matrix = new_E2;
		FOR(i, E2.rows)
		{
			FOR(j, E2.columns)
			{
				if (isinf(E2.matrix[i][j]))E2.matrix[i][j] = 2 * max1;
			}
		}
	}
	DD* E2_array = (DD*)calloc(E2.rows*E2.columns, sizeof(DD));
	bool isinf_E2 = false;
	FOR(i, E2.columns)
	{
		FOR(j, E2.rows)
		{
			if (isinf(E2.matrix[j][i]))isinf_E2 = true;
			E2_array[i*E2.rows + j] = E2.matrix[j][i];
		}
	}
	double defaultSigma = calcDefaultSigma(E2_array, 2, E2.columns*E2.rows);
	double sigma = opt.sigma * defaultSigma;
	Output_mds mds;
	if (isinf(sigma))
	{
		if (isinf_E2)
		{
			error("Cannot do the MDS: The graph is not connected");
		}
		mds = MDS(E2.matrix, E2.rows, E2.columns, opt.delta);
	}
	else
	{
		param.rbf = 1;
		double **K = calcRbfKernel(E2.matrix, E2.rows, E2.columns, param.rbf, sigma, 1);
		FOR(i, E2.rows)
		{
			FOR(j, E2.columns)
			{
				K[i][j] = 2 - 2 * K[i][j];
			}
		}
		mds = MDS(K, E2.rows, E2.columns, opt.delta);
	}
	double **cumsum = 0, sum = 0;
	cumsum = (DD**)calloc(E2.rows, sizeof(DD*));
	FOR(i, E2.rows){
		cumsum[i] = (DD*)calloc(1, sizeof(DD));
		if (i == 0)cumsum[i][0] = mds.eigen[i];
		else cumsum[i][0] = mds.eigen[i] + cumsum[i - 1][0];
	}
	sum = cumsum[E2.rows - 1][0];
	LL nb_comp;
	if (opt.delta >= 0)
	{
		FOR(i, E2.rows)
		{
			if ((1 - cumsum[i][0] < opt.delta)&(mds.eigen[i] < opt.delta*mds.eigen[0]))
			{
				nb_comp = i;
				break;
			}
		}
	}
	else
	{
		nb_comp = E2.rows;
	}
	double **Xnldr = (DD**)calloc(mds.Y.rows, sizeof(DD*));
	double *mean_Xnldr = (DD*)calloc(nb_comp, sizeof(DD));
	FOR(i, mds.Y.rows)
	{
		Xnldr[i] = (DD*)calloc(nb_comp, sizeof(DD));
		FOR(j, nb_comp){
			Xnldr[i][j] = mds.Y.matrix[i][j];
			mean_Xnldr[j] += mds.Y.matrix[i][j];
			if (i == mds.Y.rows - 1){
				mean_Xnldr[j] = (mean_Xnldr[j])/ mds.Y.rows;
			}
		}
	}
	mds.Y.columns = nb_comp;
	mds.Y.matrix = Xnldr;
	double sum_var = 0;
	FOR(i, nb_comp)
	{
		double var = 0;
		FOR(j, mds.Y.rows)
		{
			var += pow(Xnldr[j][i] - mean_Xnldr[i], 2);
		}
		sum_var += var / (mds.Y.rows - 1);
	}
	double defaultC = 1 / sum_var;
	double C = opt.C*defaultC; opt.Cinit = C*opt.Cinit; opt.Cfinal = C*opt.Cfinal; opt.C = C;
	opt.s = 3;
	double **Yu = train_one_split(mds.Y, Yl,Yl_rows,m0, opt, classes);
	return Yu;
}
double ** train_one_split(MAT Xnldr, double *Yl,long Yl_rows,long m0, Opt opt,long classes){
	MAT Y0,w0;
	w0.matrix = NULL;
	Y0.matrix = (DD**)calloc(Xnldr.rows, sizeof(DD*)); Y0.rows = Xnldr.rows; Y0.columns = 1;
	long i, j;
	if (classes == 2){
		if (Yl_rows > m0)
		{
			opt.Cinit = opt.Cfinal; opt.nofIter = 1;
		}
		else{
			FOR(i, Y0.rows)
			{
				Y0.matrix[i] = (DD*)calloc(1, sizeof(DD));
				if (i < Yl_rows)Y0.matrix[i][0] = Yl[i];
			}
			Output_Primal pr = primal_tsvm(Xnldr, Y0, w0, opt);
			int count=0;
            FOR(i,pr.Yu.rows)
			{
				double boo=(pr.Yu.matrix[i][0]>0)?1:-1;
                                if(boo==1){
                                 printf("%ld Kareena \n",i);
                                }
                                else{
                                 printf("%ld Amir \n",i);
                                 } 
                //if((pr.Yu.matrix[i][0]>0)&&(i<58))count+=1;
                //if((pr.Yu.matrix[i][0]<0)&&(i>=58))count+=1;
            }
            printf("\n total %d\n\n",count);
               return pr.Yu.matrix;
              }
	}
	else{
		MAT new_Y;
		DD final_obj=0;
		Output_Primal pr;
		new_Y.rows = Xnldr.rows - Yl_rows; new_Y.columns = classes;
		new_Y.matrix = (DD**)calloc(new_Y.rows, sizeof(DD*));
		FOR(i, new_Y.rows)new_Y.matrix[i] = (DD*)calloc(new_Y.columns, sizeof(DD));
		FOR(i, Y0.rows)
		{
			Y0.matrix[i] = (DD*)calloc(1, sizeof(DD));
		}
		FOR(i, classes)
		{
			FOR(j, Yl_rows)
			{
				if (Yl[j] == i+1)
					Y0.matrix[j][0] = 1;
				else Y0.matrix[j][0] = -1;
			}
			//print(Y0.matrix, Y0.rows, Y0.columns);
			pr = primal_tsvm(Xnldr, Y0, w0, opt);
			printf("\ndone\n");
			FOR(j, new_Y.rows)new_Y.matrix[j][i] = pr.Yu.matrix[j][0];
			final_obj += pr.obj;
		}
		print(new_Y.matrix, new_Y.rows, new_Y.columns);
        int count=0;
        FOR(i,new_Y.rows)
		{
			if(new_Y.matrix[i][0]>0 & i<57)count+=1;
            if(new_Y.matrix[i][0]<0 & i>57)count+=1;
        }
        printf("\n total %d",count);
	}
			return NULL;
}
void SVD(double*** return_mat,MAT inp){
  long i ,j;
  fmat A=randu<fmat>(inp.columns,inp.rows);
  double **my_mat=(double**)calloc(inp.columns,sizeof(double*));
  for(i=0;i<inp.columns;i++)
  {
   my_mat[i]=(double*)calloc(inp.columns,sizeof(double));
   for(j=0;j<inp.rows;j++)A(i,j)=inp.matrix[j][i];
  } 
  fmat U;
  fvec S;
  fmat V;
  svd(U, S, V, A);
  for(i=0;i<inp.columns;i++){
    for(j=0;j<inp.columns;j++)
    {
     my_mat[i][j]=U(i,j);
     }
  }
  *return_mat=my_mat;
}
Output_Primal primal_tsvm(MAT X, MAT Y, MAT w0, Opt opt)
{
	MAT new_X;
	new_X.matrix = (double**)calloc(X.rows, sizeof(double*));
	new_X.rows = X.rows; new_X.columns = X.columns + 1;
	double  C, C2, exponent, cbal = 0, sum_vbal = 0, tolfun2;
	MAT Xu, R, vbal;
	vbal.matrix = (double**)calloc(1, sizeof(double*)); vbal.rows = 1; vbal.columns = X.rows;
	vbal.matrix[0] = (double*)calloc(vbal.columns, sizeof(double));
	long i, j, zeros = 0, n, maxiter2;
	for (i = 0; i < new_X.rows; i++)
	{
		vbal.matrix[0][i] = 1;
		new_X.matrix[i] = (double*)calloc(new_X.columns, sizeof(double));
		FOR(j, new_X.columns - 1)new_X.matrix[i][j] = X.matrix[i][j];
		new_X.matrix[i][new_X.columns - 1] = 1;
	}
	for (i = 0; i < Y.rows; i++){ if (Y.matrix[i][0] == 0)zeros++; }
	Xu.matrix = (double**)calloc(zeros, sizeof(double*));
	Xu.rows = zeros;
	Xu.columns = new_X.columns;
	for (i = 0; i < Xu.rows; i++)
	{
		Xu.matrix[i] = (double*)calloc(Xu.columns, sizeof(double));
	}
	zeros = 0;
	for (i = 0; i < Y.rows; i++)
	{
		if (Y.matrix[i][0] == 0)
		{
			for (j = 0; j < Xu.columns; j++)
			{
				Xu.matrix[zeros][j] = new_X.matrix[i][j];
			}
			zeros++;
		}
	}
	Xu.matrix = Transpose(Xu.matrix, Xu.rows, Xu.columns); i = Xu.rows; Xu.rows = Xu.columns; Xu.columns = i;
	exponent = pow(opt.Cfinal / opt.Cinit, 1 / (opt.nofIter + 1));
	C = opt.C;
	C2 = opt.Cinit;
	n = new_X.columns;
	if (zeros > 0)
	{
		for (i = 0; i < Y.rows; i++)
		{
			if (Y.matrix[i][0] != 0)
			{
				vbal.matrix[0][i] = 0;
			}
		}
	}
	vbal.matrix = Multiply_Matrices(vbal.matrix, new_X.matrix, vbal.rows, new_X.columns, vbal.columns); vbal.columns = new_X.columns;
	j = 0;
	for (i = 0; i < Y.rows; i++)
	{
		if (Y.matrix[i][0] != 0)
		{
			j += 1;
			cbal += Y.matrix[i][0];
		}
	}
	for (i = 0; i < vbal.columns; i++)sum_vbal += vbal.matrix[0][i];
	cbal = (cbal / j)*sum_vbal;
    R.rows = vbal.columns; R.columns = vbal.columns;
	SVD(&R.matrix,vbal);	
	DD** newR = (double**)calloc(R.rows, sizeof(double*));
	for (i = 0; i < R.rows; i++)
	{
		newR[i] = (double*)calloc(1, sizeof(double));
		newR[i][0] = R.matrix[i][0];
	}
	MAT w;
	w.matrix = Multiply_Matrices(vbal.matrix, newR, 1, 1, R.rows);
	DD w1 = cbal / w.matrix[0][0];
	if (w0.matrix == NULL)
	{
		w.matrix = (double**)calloc(n - 1, sizeof(double*));
		for (i = 0; i < n - 1; i++)
		{
			w.matrix[i] = (double*)calloc(1, sizeof(double));
			for (j = 0; j < 1; j++)w.matrix[i][j] = 0;
		}
		w.rows = n - 1; w.columns = 1;
	}
	else
	{
		DD** new_newR = (double**)calloc(R.rows, sizeof(double*));
		for (i = 0; i < R.rows; i++)
		{
			new_newR[i] = (double*)calloc(R.columns - 1, sizeof(double));
			for (j = 0; j < R.columns - 1; j++)
			{
				new_newR[i][j] = R.matrix[i][j + 1];
			}
		}
		w.matrix = Multiply_Matrices(Transpose(new_newR, R.rows, R.columns - 1), w0.matrix, R.columns - 1, w0.rows, R.rows);
		w.rows = R.columns - 1; w.columns = w0.rows;
	}
	maxiter2 = opt.maxiter*new_X.rows;
	tolfun2 = opt.tolfun*C;
	Opt2 opt2;
	opt2.length = maxiter2;
	opt2.tolX = 0;
	opt2.tolFun = tolfun2;
	DD* y_temp = (DD*)calloc(Y.rows, sizeof(DD));
	FOR(i, Y.rows)
	{
		y_temp[i] = Y.matrix[i][0];
	}
	bool FLAG;
	Output_Obj  oo;
	FOR(i, opt.nofIter)
	{
		C2 = C2*exponent;
		Output_mini tt = minimize(w, w1, y_temp, Y.rows, C, C2, opt.s, new_X, Xu, R, opt2);
		oo = obj_fun(w, w1, y_temp, Y.rows, C, C2, opt.s,new_X,Xu,R);
		FLAG = (tt.FX < opt2.length) ? 1 : 0;
	}
	DD **Temp=(DD**)calloc(w.rows+1,sizeof(DD*));
	FOR(i, w.rows + 1)
	{
		Temp[i] = (DD*)calloc(1, sizeof(DD));
		if (i !=0)Temp[i][0] = w.matrix[i-1][0];
		else Temp[i][0] = w1;
	}
	w.matrix = Multiply_Matrices(R.matrix, Temp, R.rows, 1, w.rows + 1); w.rows = R.rows; w.columns = 1;
	MAT Yu;
	Yu.matrix = Multiply_Matrices(Transpose(Xu.matrix, Xu.rows, Xu.columns), w.matrix, Xu.columns, w.columns, w.rows); Yu.rows = Xu.columns; Yu.columns = w.columns;
	Output_Primal pr;
	pr.obj = oo.obj;
	pr.b = w.matrix[w.rows - 1][0];
	pr.Yu = Yu;
	free(w.matrix[w.rows - 1]);
	w.rows = w.rows - 1;
	pr.w = w;
	return pr;
}

int main(){
  long rows=2500,columns=20,columns2=201,i,j;	
  FILE *fp;
  fp=fopen("final_file_train.txt","r");
  DD** k=(DD**)calloc(rows,sizeof(DD*));
  FOR(i,rows)
  {
	k[i]=(DD*)calloc(columns,sizeof(DD));	
  }
  FOR(j,columns)
  {
	  FOR(i,rows)
	  {
	   fscanf(fp,"%lf ",&k[i][j]);
	  }
  }
  fclose(fp);
  fp=fopen("final_file_test.txt","r");
  DD** k2=(DD**)calloc(rows,sizeof(DD*));
  FOR(i,rows)
  {
	k2[i]=(DD*)calloc(columns2,sizeof(DD));	
  }
  FOR(j,columns2)
  {
	  FOR(i,rows)
	  {
	     fscanf(fp,"%lf ",&k2[i][j]);
	  }
  } 
  fclose(fp);
  DD* Yl=(DD*)calloc(columns,sizeof(DD*));
  FOR(i,20)
  {
	  if(i<10)
	     Yl[i]=-1;
	  else 
	     Yl[i]=1;
  }
  Opt opt;
  opt.delta = 0.1;
  opt.nofIter = 10;
  opt.C = 2;
  opt.Cinit = 0.01;
  opt.Cfinal = 1;
  opt.nofNn = 50;
  opt.sigma = INFINITY;
  opt.opt_tb = 1;
  opt.maxiter = 3;
  opt.tolfun = exp(-5);
  DD** y=LDS(k,rows,columns,k2,rows,columns2,Yl,columns,2,0.5,opt);
  return 0;
}
