#include "Graph.c"
#include "utility.c"

/*  Adsorption function for semi supervised learning based on the algorithm explained in the paper :-
    New Regularized Algorithms for Transductive Learning by Partha Pratim Talukdar and Koby Crammer*/
DD** AD(DD** W,LL Rows,LL Classes, DD* Labels)
{
	LL i=0,j=0,k=0,l=0;
	DD Beta=2,Meu1=4,Meu2=2,Meu3=2,sum=0;
	//Initialize Y,Y_cap and R matrices
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
	//Calculate Pr Matrix
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
    
    //Calculate Matrix H
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
    FOR(i,Rows)free(Pr[i]);
    free(Pr);
    
    //Calculate Matrices C, D and Z for calculating probabilities
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
    
    //Find Probabilites P_inj, P_cont and P_abnd
    DD** P_inj=Zeros(Rows,1);
    DD** P_cont=Zeros(Rows,1);
    DD** P_abnd=Zeros(Rows,1);
    FOR(i,Rows)
    {
       P_inj[i][0]=D[i][0]/Z[i][0];
       P_cont[i][0]=C[i][0]/Z[i][0];
       P_abnd[i][0]=1-P_inj[i][0]-P_cont[i][0];
    }
    
   //free up memory of matrices 
   FOR(i,Rows)free(C[i]);
   free(C);
   FOR(i,Rows)free(D[i]);
   free(D);
   FOR(i,Rows)free(Z[i]);
   free(Z);
   FOR(i,Rows)free(H[i]);
   free(H);
   
   // Iterations
   FOR(k,500)
   {
   DD sum=0;	
   D=Zeros(Rows,Classes+1); 
   DD**Y_old=Copy(Y_cap,Rows,Classes+1); 
   
   //Calculate D matrix
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
   FOR(i,Rows)
   {
   	   FOR(l,Classes+1)
   	   {
       Y_cap[i][l]=(Meu1*P_inj[i][0]*Y[i][l]+Meu2*P_cont[i][0]*D[i][l]+Meu3*P_abnd[i][0]*R[0][l]);
       }
   }	
   if((fabs(pow(norm(Y_cap,Rows,Classes+1),2)-pow(norm(Y_old,Rows,Classes+1),2)))<=(0.001))
       break;
   }
   DD**results=Zeros(Rows,1);
   FOR(i,Rows)
   {
   	DD max=Y_cap[i][0];
   	results[i][0]=0;
   	FOR(j,Classes)
   	{
   		if(Y_cap[i][j]>max)
   		{
   		results[i][0]=j;
		max=Y_cap[i][j];   	
   		}
   	}
   }
   FOR(i,Rows){
   free(Y_cap[i]);
   free(Y[i]);
   }
   free(Y_cap);
   free(Y);
   return results; 
}

