#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#define FOR(i,n) for(i=0;i<n;i++) 
typedef double DD;
typedef long LL;
/* function returning the max between two numbers */
DD max(DD num1, DD num2) 
{
   DD result;
 
   if (num1 > num2)
      result = num1;
   else
      result = num2;
 
   return result; 
}
/* function returning the zero matrix with dimension RowsXCols*/
DD** Zeros(LL Rows, LL Cols)
{
    LL i,j=0;
	DD** Return_mat=(DD**)calloc(Rows,sizeof(DD*));
	FOR(i,Rows)
	    Return_mat[i]=(DD*)calloc(Cols,sizeof(DD));	
	return Return_mat;;    
}
DD** Copy(DD** Matrix,LL Rows,LL Cols)
{
	LL i,j=0;
	DD** Return_mat=(DD**)calloc(Rows,sizeof(DD*));
	FOR(i,Rows)
	   {
	   Return_mat[i]=(DD*)calloc(Cols,sizeof(DD));	
	    FOR(j,Cols)
	    Return_mat[i][j]=Matrix[i][j];
	   }
	return Return_mat;    
}
void ADD_Rows(DD* Row1,DD*Row2,LL length,DD Scalar)
{
     LL i=0;
	 FOR(i,length)	
	 {
	 	Row1[i]+=Scalar*Row2[i];
	 }
}
DD norm(DD** Row1,LL Rows,LL length)
{
     LL i=0,j=0;
     DD sum=0;
     FOR(i,Rows)
     {
	 FOR(j,length)	
	 {
	 	sum+=pow(Row1[i][j],2);
	 }
	 }
	 return sqrt(sum);
}
DD** MAD(DD** W,LL Rows,LL Classes, DD* Labels)
{
	LL i=0,j=0,k=0,l=0;
	DD Beta=2,Meu1=4,Meu2=4,Meu3=2,sum=0;
	DD** Y=Zeros(Rows,Classes+1);
	FOR(i,Rows)
	   FOR(j,Classes+1)
    	{
	    	if(Labels[i]==j)
		        Y[i][j]=1;
		    else
		        Y[i][j]=0;
	   }
	DD**Y_cap=Copy(Y,Rows,Classes+1);   
	DD** R=Zeros(1,Classes+1);
	R[0][Classes]=1;
	DD** Pr=Zeros(Rows,Rows);
	FOR(i,Rows)
	{
	    sum=0;	
	    FOR(j,Rows)
	    {
		   if(W[i][j]!=0)
		   {
		     sum+=W[i][j];
		   }
    	}
	    FOR(j,Rows)
	    {
		   Pr[j][i]=W[i][j]/sum;
	    }
    }
    
    DD** H=Zeros(Rows,1);
 
    FOR(i,Rows)   
    {
    	sum=0;
		FOR(j,Rows)
    	{
    		if( (W[i][j]!= 0) && (Pr[i][j] > 0) )  
    		{
    		sum+=-1*Pr[i][j]*log(Pr[i][j]);
    		}
    	}
    	H[i][0]=sum;
    }
    DD** C=Zeros(Rows,1);
    FOR(i,Rows)
     C[i][0]=log(Beta)/log(Beta+exp(H[i][0]));
     
    DD** D=Zeros(Rows,1);
    DD** Z=Zeros(Rows,1);
    FOR(i,Rows)
    {
      if(Labels[i]>=0)
        D[i][0]=(1-C[i][0])*(pow(H[i][0],0.5));
      else
        D[i][0]=0;  
      Z[i][0]=max(D[i][0]+C[i][0],1);
    }
   /*  FOR(i,3)
	{
		FOR(j,1)
		printf("%e ",D[i][j]);
		printf("\n");
	}*/ 
    DD** P_inj=Zeros(Rows,1);
    DD** P_cont=Zeros(Rows,1);
    DD** P_abnd=Zeros(Rows,1);
    FOR(i,Rows)
    {
       P_inj[i][0]=D[i][0]/Z[i][0];
       P_cont[i][0]=C[i][0]/Z[i][0];
       P_abnd[i][0]=1-P_inj[i][0]-P_cont[i][0];
    }
    DD** M=Zeros(3,1);
    //DD** M;
    FOR(i,Rows)
    {
    sum=0;
    FOR(j,Rows)
     {
        if(i!=j)
           sum=sum+P_cont[i][0]*W[i][j] + P_cont[j][0]*W[j][i];
     }
    M[i][0]=Meu1*P_inj[i][0]+Meu2*sum+Meu3;
    }
    /*FOR(i,3)
	{
		FOR(j,1)
		printf("%e ",P_inj[i][j]);
		printf("\n");
	} 
	 FOR(i,3)
	{
		FOR(j,1)
		printf("%e ",P_cont[i][j]);
		printf("\n");
	} 
	 FOR(i,3)
	{
		FOR(j,1)
		printf("%e ",P_abnd[i][j]);
		printf("\n");
	}  
   FOR(i,3)
	{
		FOR(j,1)
		printf("%e ",M[i][j]);
		printf("\n");
	} */
   FOR(k,500)
   {
   DD sum=0;	
   D=Zeros(Rows,Classes+1); 
   DD**Y_old=Copy(Y_cap,Rows,Classes+1); 
  /* FOR(i,3)
	{
		FOR(j,3)
		printf("%e ",Y_cap[i][j]);
		printf("\n");
	}   */
   FOR(i,Rows)
   {
   	sum=0;
    FOR(j,Rows)
    {
        if(W[i][j]>0)
           ADD_Rows(D[i],Y_cap[j],Classes+1,W[j][i]);
           sum+=W[j][i];
    }  
    FOR(j,Classes+1)  
    {
    	if(sum!=0)
    	D[i][j]=D[i][j]/sum;
    }
   }
   /*
   FOR(i,3)
	{
		FOR(j,3)
		printf("%e ",D[i][j]);
		printf("\n");
	}*/ 
   FOR(i,Rows)
   {
   	   FOR(l,Classes+1)
   	   {
       Y_cap[i][l]=(Meu1*P_inj[i][0]*Y[i][l]+Meu2*D[i][l]+Meu3*P_abnd[i][0]*R[0][l]);
       }
   }
   FOR(i,3)
	{
		FOR(j,3)
		printf("%e ",Y_cap[i][j]);
		printf("\n");
	}
   printf("\n");	
   /*if(k==27)
   {
   	printf("\n\t values %e %e\n",norm(Y_cap,Rows,Classes+1),norm(Y_old,Rows,Classes+1));
   }*/
   if((fabs(pow(norm(Y_cap,Rows,Classes+1),2)-pow(norm(Y_old,Rows,Classes+1),2)))<=(0.001))
       break;
   }
   return Y_cap; 
}
// arrange labels in descending order as in first give 0 labels then 1
int main(){
	LL i,j;
	DD** k=(DD**)calloc(3,sizeof(DD*));
	FOR(i,3)
	    k[i]=(DD*)calloc(3,sizeof(DD));	
	k[0][0]=1;k[0][1]=2;k[0][2]=3;
	k[1][0]=2;k[1][1]=4;k[1][2]=6;
	k[2][0]=3;k[2][1]=6;k[2][2]=9;
	DD* labels=(DD*)calloc(3,sizeof(DD));
	labels[0]=0;labels[1]=1;labels[2]=-1;
	DD** n=MAD(k,3,2,labels);
	/*FOR(i,3)
	{
		FOR(j,3)
		printf("%e ",n[i][j]);
		printf("\n");
	}*/
	return 0;
}
