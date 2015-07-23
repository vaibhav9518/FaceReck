#include <stdio.h>
#include<math.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdbool.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h> 
#define FOR(i,n) for(i=0;i<n;i++)
#define MAX_IT 1000
#define r_Max 24
#define k_Const 1.8
#define Class_Number 2
#define Class_Size 100
#define Dimension 784
#define Test_Number 500
#define Ratio 2000
typedef double DD, LD;
typedef long LL;
LL i, Upper_Limit;
DD r_Kernel = 2;
bool Current_Gram = false;
DD** Global_Q = NULL;
DD** Global_Temp = NULL;
DD** adjoint(DD **a, LL n);
DD* Copy_Row(DD* Row, LL N);
DD** Precise(DD** Mat, long Rows, long Columns);
DD** Transpose(DD** Mat1, LL Rows, LL Columns);
DD ABC = 0;
void Subtract_Matrices(DD** Mat1, DD** Mat2, LL rows, LL columns);
DD** Multiply_Matrices(DD** Mat1, DD** Mat2, LL M, LL Q, LL P);
DD determinant(DD **a, long n);
void Subtract_Vectors(DD *a1, DD *a2, LL N);
void Multiply_Row(DD *Row, LL N, DD k);

void Subtract_Rows(DD *Row1, DD *Row2, LL N)
{
	LL i;
	FOR(i, N)
		Row1[i] -= Row2[i];
}
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

long find_eigen2(double **mat, long n, double***vec, double**values)
{
	long i, j, count = 0;
	double *data = (double*)calloc(((long)(n*n)), sizeof(double));
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			data[i*n + j] = mat[i][j];
		}
	}
	gsl_matrix_view m
		= gsl_matrix_view_array(data, n, n);

	gsl_vector_complex *eval = gsl_vector_complex_alloc(n);
	gsl_matrix_complex *evec = gsl_matrix_complex_alloc(n, n);

	gsl_eigen_nonsymmv_workspace * w =
		gsl_eigen_nonsymmv_alloc(n);

	gsl_eigen_nonsymmv(&m.matrix, eval, evec, w);

	gsl_eigen_nonsymmv_free(w);

	gsl_eigen_nonsymmv_sort(eval, evec,
		GSL_EIGEN_SORT_ABS_DESC);
	{
		int i;

		for (i = 0; i < n; i++)
		{
			gsl_complex eval_i
				= gsl_vector_complex_get(eval, i);
			double check = GSL_IMAG(eval_i);
			if (!check)count++;
		}
	}
	double *eig_values = (double*)calloc(count, sizeof(double));
	double** eig_vec = (double**)calloc(n, sizeof(double*));
	for (i = 0; i<n; i++)eig_vec[i] = (double*)calloc(count, sizeof(double));
	{
		int i, j,l;

		for (i = 0, l = 0; i < n; i++)
		{
			gsl_complex eval_i
				= gsl_vector_complex_get(eval, i);
			double check = GSL_IMAG(eval_i);
			if (!check){
				eig_values[l] = GSL_REAL(eval_i);
				for (j = 0; j<n; j++)
				{
					gsl_vector_complex_view evec_i
						= gsl_matrix_complex_column(evec, i);
					eig_vec[j][l] = GSL_REAL(gsl_vector_complex_get(&evec_i.vector, j));
				}
				l++;
			}
		}
	}
	*vec = eig_vec;
	*values = eig_values;
	gsl_vector_complex_free(eval);
	gsl_matrix_complex_free(evec);
	free(data);
	return count;
}

double ** H_mat(long n)
{
	long i, j;
	DD** k = (DD**)calloc(n, sizeof(DD*));
	FOR(i, n)
	{
		k[i] = (DD*)calloc(n, sizeof(DD));
	}
	FOR(i, n){
		FOR(j, n)k[i][j] = (i == j) ? (1.00 - 1.00 / n) : (-1.00 / n);
    }
	return k;
}
int main(){
	long rows = 3, columns = 3, i, j,n,magn_keep=0,max_val=-1*pow(10,320);
	DD  **temp,k,**D,*val,*idx,*keep,delta=-1,**H,**new_D,**K,**new_K,**ind,**V,**mult_V,**Y;
	D=(DD**)calloc(rows, sizeof(DD*));
	new_D= (DD**)calloc(rows, sizeof(DD*));
	idx=(DD*)calloc(n, sizeof(DD));
	keep=(DD*)calloc(n, sizeof(DD));
	V= (DD**)calloc(n, sizeof(DD*));
	FOR(i, rows)
	{
		D[i] = (DD*)calloc(columns, sizeof(DD));
		new_D[i]=(DD*)calloc(columns, sizeof(DD));
		
	}
	D[0][0] = 1; D[0][1] = 2;D[0][2]=3;
	D[1][0] = 2; D[1][1] = 4;D[1][2]=6;
	D[2][0] = 3; D[2][1] = 6;D[2][2]=9;
	//FOR(i,rows)
	//FOR(j,columns)new_D[i][j]=-0.5*D[i][j];
	
	n = max(rows, columns);
	if(delta>=0)
	{
		H=H_mat(n);
		new_K= (DD**)calloc(n, sizeof(DD*));
		FOR(i, rows)
	    {   
		new_k[i]=(DD*)calloc(n, sizeof(DD));
	    }
		FOR(i,rows)
		{
			FOR(j,columns)
			new_D[i][j]=-0.5*D[i][j];
		}
		K=Multiply_Matrices(H,Multiply_Matrices(new_D,H,rows,n,n), n, rows, n);
		FOR(i,n)
		{
		    idx[i]=i+1;
			FOR(j,rows)
			new_k[i][j]=(k[i][j]+k[j][i])/2;
		}
		find_eigen2(new_k,n,&temp, &val);
		FOR(i,n)val[i]=-val[i];
		for (i = 1 ; i <= n - 1; i++) 
		{
           j = i;
           while ( j > 0 && val[j] < val[j-1]) 
		   {
            t = val[j];
            val[j]   = val[j-1];
            val[j-1] = t;
            t= idx[j];
            idx[j]= idx[j-1];
            idx[j-1] = t;
            j--;
            }
        }
        FOR(i,n)
		{
		val[i]=-val[i];
		if(val[i]>max_val)max_val=val[i];
	    }
        FOR(i,n){
        	if(val[i]>pow(2.3,-10)*max_val){
			keep[magn_keep]=i;
        	magn_keep++;
            }
        }
        mult_V= (DD**)calloc(magn_keep, sizeof(DD*));
        FOR(i, n)V[i] = (DD*)calloc(magn_keep, sizeof(DD));
        FOR(i,magn_keep)
		{
		mult_V[i]=(DD*)calloc(magn_keep, sizeof(DD));
		mult_V[i][i]=val[keep[i]];
		FOR(j,n)
		  {
		    V[j][i]=temp[j][keep[i]];
		  }
	    }
        Y=Multiply_Matrices(V,mult_V,n,magn_keep,magn_keep);
	}
	
	FOR(i, rows)
	{
		printf("%e ", val[i]);
		FOR(j, rows)
		{
		}
		printf("\n");
	}
	H = Multiply_Matrices(D, D, 3, 3, 3);
	
	FOR(i, rows)
	{
		FOR(j, rows)
		{
			printf("%e ", H[i][j]);
		}
		printf("\n");
	}
	
	system("pause");
	return 0;
}
