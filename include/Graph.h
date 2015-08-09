#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#define FOR(i,n) for(i=0;i<n;i++) 
typedef double DD;
typedef long LL;

/* ---- quicksort function to sort a 2D points on the basis
       of first coordinate passed in the form of 2D array ----*/
       
void quicksort(double** x,long first,long last){
    long pivot,j,i;
    double temp;
     if(first<last){
         pivot=first;
         i=first;
         j=last;
         while(i<j){
             while(x[0][i]<=x[0][pivot]&&i<last)
                 i++;
             while(x[0][j]>x[0][pivot])
                 j--;
             if(i<j){
                 temp=x[0][i];
                 x[0][i]=x[0][j];
                 x[0][j]=temp;
                 temp=x[1][i];
                 x[1][i]=x[1][j];
                 x[1][j]=temp; 
             }
         }
         temp=x[0][pivot];
         x[0][pivot]=x[0][j];
         x[0][j]=temp;
         temp=x[1][pivot];
         x[1][pivot]=x[1][j];
         x[1][j]=temp;
         quicksort(x,first,j-1);
         quicksort(x,j+1,last);

    }
}

/* ---- Function to return cosine similarity measure 
        between two vectors a and b passed as parameters ---- */
        
DD Cosine_similarity(DD* a,DD* b,LL length){
	LL i;
	DD sum=0,sum2=0,sum3=0;
	FOR(i,length){
    	sum+=a[i]*b[i];
	}
	FOR(i,length){
    	sum2+=pow(a[i],2);
    	sum3+=pow(b[i],2);
	}
	sum2=pow(sum2,0.5);
	sum3=pow(sum3,0.5);
	return sum/(sum2*sum3);
}

/* ---- Function to return Pearson correlation similarity measure
        between two vectors a and b passed as parameters ---- */
        
DD pearson_correlation(DD* a,DD* b,LL length){
	LL i;
	DD sum=0,sum2=0,sum3=0,avg1=0,avg2=0;
	FOR(i,length){
    	avg1+=a[i];
    	avg2+=b[i];
	}
	avg1=avg1/length;
	avg2=avg2/length;
	FOR(i,length){
    	sum+=(a[i]-avg1)*(b[i]-avg2);
	}
	FOR(i,length){
    	sum2+=pow(a[i]-avg1,2);
    	sum3+=pow(b[i]-avg2,2);
	}
	sum2=pow(sum2,0.5);
	sum3=pow(sum3,0.5);
	return sum/(sum2*sum3);
}

/*---- Function that returns the weight matrix from Matrix: mat passed as 
       parameter by using its rows as vectors and each (i,j) entry of 
	   weight matrix is equal to similarity measure between ith and jth 
	   vector calculated after finding nearest neighbours using cosine similarity ----*/
	   
DD** generate_weights_cosine(DD** mat,LL Rows,LL Columns,LL NN){
	LL i,j;
	DD** temp=(DD**)calloc(2,sizeof(DD*));
	FOR(i,2)
	    temp[i]=(DD*)calloc(Rows,sizeof(DD));	
    DD** Return_mat=(DD**)calloc(Rows,sizeof(DD*));
	FOR(i,Rows)
	    Return_mat[i]=(DD*)calloc(Columns,sizeof(DD));	
	FOR(i,Rows)
	{
		FOR(j,Rows)
		{
			temp[0][j]=Cosine_similarity(mat[i],mat[j],Columns);
			temp[1][j]=j;
		}
		quicksort(temp,0,Rows-1);
		FOR(j,NN)
		  Return_mat[i][(int)temp[1][Rows-1-j]]=temp[0][Rows-1-j];
	}
	FOR(i,2)free(temp[i]);
	free(temp);
	return Return_mat;
}

/*---- Function that returns the weight matrix from Matrix: mat passed as parameter 
       by using its rows as vectors and each (i,j) entry of weight matrix is equal to
	   similarity measure between ith and jth vector calculated after finding 
	   nearest neighbours using pearson correlation similarity ----*/
	   
DD** generate_weights_pearson(DD** mat,LL Rows,LL Columns,LL NN){
	LL i,j;
	DD** temp=(DD**)calloc(2,sizeof(DD*));
	FOR(i,2)
	    temp[i]=(DD*)calloc(Rows,sizeof(DD));	
    DD** Return_mat=(DD**)calloc(Rows,sizeof(DD*));
	FOR(i,Rows)
	    Return_mat[i]=(DD*)calloc(Columns,sizeof(DD));	
	FOR(i,Rows)
	{
		FOR(j,Rows)
		{
			temp[0][j]=pearson_correlation(mat[i],mat[j],Columns);
			temp[1][j]=j;
		}
		quicksort(temp,0,Rows-1);
		FOR(j,NN)
		  Return_mat[i][(int)temp[1][Rows-1-j]]=temp[0][Rows-1-j];
	}
	FOR(i,2)free(temp[i]);
	free(temp);
	return Return_mat;
}

