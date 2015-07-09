#include <stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdbool.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h> 
#define FOR(i,n) for(i=0;i<n;i++)
typedef double DD, LD;
typedef long LL;

void quicksort_mds(double* x, long first, long last)
{
	long pivot, j, i;
	double temp;
	if (first<last)
	{
		pivot = first;
		i = first;
		j = last;
		while (i<j)
		{
			while (x[i]<=x[pivot] && i<last)
				i++;
			while (x[j]>x[pivot])
				j--;
			if (i<j)
			{
				temp = x[i];
				x[i] = x[j];
				x[j] = temp;
			}
		}
		temp = x[pivot];
		x[pivot] = x[j];
		x[j] = temp;
		quicksort(x, first, j - 1);
		quicksort(x, j + 1, last);

	}
}

void find_eigen(double **mat, int n, double*** res_vec, double** res_val)
{
	int i, j;
	double *data = (double*)calloc(((long)(n*n)), sizeof(double));
	double *eig_values = (double*)calloc(n, sizeof(double));
	double** eig_vec = (double**)calloc(n, sizeof(double*));
	for (i = 0; i<n; i++)eig_vec[i] = (double*)calloc(n, sizeof(double));
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			data[i*n + j] = mat[i][j];
		}
	}
	gsl_matrix_view m= gsl_matrix_view_array(data, n, n);
	gsl_vector *eval = gsl_vector_alloc(n);
	gsl_matrix *evec = gsl_matrix_alloc(n, n);
	gsl_eigen_symmv_workspace * w =gsl_eigen_symmv_alloc(n);
	gsl_eigen_symmv(&m.matrix, eval, evec, w);
	gsl_eigen_symmv_free(w);
	gsl_eigen_symmv_sort(eval, evec,GSL_EIGEN_SORT_VAL_ASC);
	for (i = 0; i<n; i++)
	{
		eig_values[i] = gsl_vector_get(eval, i);
		for (j = 0; j<n; j++)
		{
			eig_vec[i][j] = gsl_matrix_get(evec, i, j);
		}
	}
	*res_val = eig_values;
	*res_vec = eig_vec;
	gsl_vector_free(eval);
	gsl_matrix_free(evec);
	free(data);
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
	gsl_matrix_view m= gsl_matrix_view_array(data, n, n);
	gsl_vector_complex *eval = gsl_vector_complex_alloc(n);
	gsl_matrix_complex *evec = gsl_matrix_complex_alloc(n, n);
	gsl_eigen_nonsymmv_workspace * w =gsl_eigen_nonsymmv_alloc(n);
	gsl_eigen_nonsymmv(&m.matrix, eval, evec, w);
	gsl_eigen_nonsymmv_free(w);
	gsl_eigen_nonsymmv_sort(eval, evec,GSL_EIGEN_SORT_ABS_DESC);
	{
		int i;
		for (i = 0; i < n; i++)
		{
			gsl_complex eval_i= gsl_vector_complex_get(eval, i);
			double check = GSL_IMAG(eval_i);
			if (!check)count++;
		}
	}
	double *eig_values = (double*)calloc(count, sizeof(double));
	double** eig_vec = (double**)calloc(n, sizeof(double*));
	for (i = 0; i<n; i++)eig_vec[i] = (double*)calloc(count, sizeof(double));
	{
		int i, j, l;
		for (i = 0, l = 0; i < n; i++)
		{
			gsl_complex eval_i= gsl_vector_complex_get(eval, i);
			double check = GSL_IMAG(eval_i);
			if (!check)
			{
				eig_values[l] = GSL_REAL(eval_i);
				for (j = 0; j<n; j++)
				{
					gsl_vector_complex_view evec_i= gsl_matrix_complex_column(evec, i);
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


DD** Multiply_Matrices_mds(DD **Mat1, DD **Mat2, LL M, LL Q, LL P)
{
	LL i, c, d, k;
	DD Sum = 0;
	DD** Temp = (DD**)calloc(M, sizeof(DD*));
	FOR(i, M)Temp[i] = (DD*)calloc(Q, sizeof(DD));
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

double ** H_mat(long n)
{
	long i, j;
	DD** k = (DD**)calloc(n, sizeof(DD*));
	FOR(i, n)
	{
		k[i] = (DD*)calloc(n, sizeof(DD));
	}
	FOR(i, n){
		FOR(j, n){ k[i][j] = (i == j) ? (1.00 - 1.00 / n) : (-1.00 / n);
		}
    }
	return k;
}
typedef struct{
	MAT Y;
	DD* eigen;
	LL number;
} Output_mds;
Output_mds MDS(DD** D,long rows,long columns, DD delta)
{
	long  i, j, n;
	DD    **H, **new_D, **K, **new_k;
	new_D = (DD**)calloc(rows, sizeof(DD*));
	n = max(rows, columns);
	FOR(i, rows)
	{
		new_D[i] = (DD*)calloc(columns, sizeof(DD));
	}
	if (delta >= 0)
	{
		H = H_mat(n);
		new_k = (DD**)calloc(n, sizeof(DD*));
		FOR(i, rows)
		{
			new_k[i] = (DD*)calloc(n, sizeof(DD));
		}
		FOR(i, rows)
		{
			FOR(j, columns)
				new_D[i][j] = -0.5*D[i][j];
		}
		K = Multiply_Matrices_mds(Multiply_Matrices_mds(H, new_D, n, columns, n), H, columns, n, n);
        FOR(i,rows)free(new_D[i]);free(new_D);
        FOR(i,n)free(H[i]);free(H);  
		DD* idx = (DD*)calloc(n, sizeof(DD));
		DD  ** temp, *val;
		FOR(i, n)
		{
			idx[i] = n - i;
			FOR(j, rows)
				new_k[i][j] = (K[i][j] + K[j][i]) / 2;
		}
		//FOR(i,n)free(K[i]);free(K);
		find_eigen(new_k, n, &temp, &val);
		//FOR(i,n)free(new_k[i]);free(new_k);
		j = n - 1;
		for (i = 0; i < n / 2; i++) 
		{
			DD temporary = val[i];
			val[i] = val[j];
			val[j] = temporary;
			j--;
		}
		LL* keep;
		DD max_val = val[0];
		LL magn_keep = 0;
		FOR(i, n){
			if (val[i] > max_val)max_val = val[i];
		}
		max_val = max_val*exp(-10);
		FOR(i, n){
			if (val[i] > max_val)magn_keep++;
		}
		keep = (LL*)calloc(magn_keep, sizeof(LL));
		magn_keep = 0;
		FOR(i, n){
			if (val[i] > max_val){
				keep[magn_keep++] = i;
			}
		}
		DD** V = (DD**)calloc(n, sizeof(DD*));
		DD** mult_V = (DD**)calloc(magn_keep, sizeof(DD*));
		FOR(i, n)V[i] = (DD*)calloc(magn_keep, sizeof(DD));
		FOR(i, magn_keep)
		{
			mult_V[i] = (DD*)calloc(magn_keep, sizeof(DD));
			FOR(j, n)
			{
				if (j < magn_keep)
				{
					if (i != j)mult_V[i][j] = 0;
					else mult_V[i][j] = sqrt(val[keep[i]]);
				}
				V[j][i] = temp[j][(int)idx[keep[i]]-1];
			}
		}
		V = Multiply_Matrices_mds(V, mult_V, n, magn_keep, magn_keep);
        //FOR(i,magn_keep)free(mult_V[i]);
        //free(mult_V);
        //free(keep);free(idx);  
		MAT return_MAT;
		return_MAT.matrix = V;
		return_MAT.rows = n;
		return_MAT.columns = magn_keep;
		Output_mds t;
		t.Y = return_MAT; t.eigen = val; t.number=n;
		return t;
	}

}
	
