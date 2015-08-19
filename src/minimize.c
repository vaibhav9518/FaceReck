#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Obj_fun.h"
#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

typedef struct{
	LL FX;
	LL count;
} Output_mini;

Output_mini minimize(MAT w_in,double w1,double *Y,long Y_rows,double C,double C2,double S,MAT X,MAT Xu,MAT R,Opt2 opt2){
	Output_mini ret_mini;
	long i1,j1,FX=0;
	DD RHO=0.01,SIG=0.5,INT=0.1,EXT=3,MAX=20,RATIO=100;
    LL length=opt2.length;
	//assuming	max(size(length)) == 2 is never true
	DD red=1;
	DD** temp;
	LL i=0,is_failed=0;// zero the run length counter
	Output_Obj F=obj_fun(w_in,w1,Y,Y_rows,C,C2,S,X,Xu,R);//get function value and gradient
	i=(length<0)?i+1:i;
    MAT s;
    s.matrix=copy(F.mat.matrix,F.mat.rows,F.mat.columns,-1);
    s.rows=F.mat.rows;s.columns=F.mat.columns;
    DD** D1=Multiply_Matrices(Transpose(s.matrix,s.rows,s.columns),s.matrix,s.columns,s.columns,s.rows);	
    DD d1=-D1[0][0];
    // this is the slope
    DD z1=red/(1-d1);
    DD A,B;
    while(i<abs(length))
    {
		      i=(length>0)?i+1:i;
		      // make a copy of current values
			  double **X0=copy(w_in.matrix,w_in.rows,w_in.columns,1);
			  double f0=F.obj;
			  double **df0=copy(F.mat.matrix,F.mat.rows,F.mat.columns,1);
			  temp=copy(s.matrix,s.rows,s.columns,z1);
			  for(i1=0;i1<w_in.rows;i1++)
			  {
			  	FOR(j1,w_in.columns)
			  	{
			  		w_in.matrix[i1][j1]+=temp[i1][j1];
			  	}
			  }
			  Output_Obj F2=obj_fun(w_in,w1,Y,Y_rows,C,C2,S,X,Xu,R);
			  i=(length<0)?i+1:i;
			  double **D2=Multiply_Matrices(Transpose(F2.mat.matrix,F2.mat.rows,F2.mat.columns),s.matrix,F2.mat.columns,s.columns,s.rows);
			  double d2=D2[0][0];
			  //initialize point 3 equal to point 1
			  double f3=F.obj;double d3=d1;double z3=-1*z1;
			  double M=(length>0)?MAX:min(MAX,-length-i);
			  double success=0,limit=-1;
			  DD z2;
			  while(1)
			  {
					  	while(((F2.obj>F.obj+z1*RHO*d1)|(d2>-SIG*d1))&(M>0))
					  	{
					  		limit=z1;
					  		if(F2.obj>F.obj)
					  		{
					  			z2 = z3 - (0.5*d3*z3*z3)/(d3*z3+F2.obj-f3);//quadratic fit
					  		}
					  		else{
					  		    A = 6*(F2.obj-f3)/z3+3*(d2+d3);//cubic fit
				                B = 3*(f3-F2.obj)-z3*(d3+2*d2);
				                if((B*B-A*d2*z3*z3)<0)
								{
								printf("error -ve value in sqaure root");
								getchar();
							    }
				                z2 = (sqrt(B*B-A*d2*z3*z3)-B)/A; //this step can cause problems      
					  		}
					  		if(isinf(z2)|isnan(z2))z2=z3/2;//if we had a numerical problem then bisect
					  		z2 = max(min(z2, INT*z3),(1-INT)*z3);  
				            z1 = z1 + z2;
							FOR(i1,w_in.rows)                          
							{
									FOR(j1,w_in.columns)
									{
								      w_in.matrix[i1][j1] = w_in.matrix[i1][j1] + z2*s.matrix[i1][j1];		
									}
							}
							F2=obj_fun(w_in,w1,Y,Y_rows,C,C2,S,X,Xu,R);
				            M = M - 1; 
							i = (length<0)?i+1:i;        
					        D2=Multiply_Matrices(Transpose(F2.mat.matrix,F2.mat.rows,F2.mat.columns),s.matrix,F2.mat.columns,s.columns,s.rows); 
					        d2=D2[0][0];
				            z3 = z3-z2;//z3 is now relative to the location of z2
						}
						if((F2.obj > (F.obj+z1*RHO*d1)) ||( d2 > (-SIG*d1)))
						{
							break;//this is a failure
						}
				        else if(d2 > (SIG*d1))
						{
								  success = 1; //success
							      break;
				        }
				        else if( ((int)M) == 0)break;   //failure   
						else{} 
						A = 6*(F2.obj-f3)/z3+3*(d2+d3);  // make cubic extrapolation
				        B = 3*(f3-F2.obj)-z3*(d3+2*d2);
				        z2 = -d2*z3*z3/(B+sqrt(B*B-A*d2*z3*z3));        
				        if (isnan(z2) | isinf(z2) | z2 < 0)   // not checked for complex
				        {
						    if(limit < -0.5) z2 = z1 * (EXT-1);                
				            else z2 = (limit-z1)/2;                
				        }
				        else if((limit > -0.5) & (z2+z1 > limit))
				                 z2 = (limit-z1)/2;                  
				        else if ((limit < -0.5) & (z2+z1 > z1*EXT))
				                 z2 = z1*(EXT-1.0);                  
				        else if (z2 < -z3*INT)
				                 z2 = -z3*INT;
				        else if ((limit > -0.5) & (z2 < (limit-z1)*(1.0-INT)))
				                 z2 = (limit-z1)*(1.0-INT);
				        else{
				            }
				        f3 = F2.obj; d3 = d2; z3 = -z2;     
				        z1 = z1 + z2;     
				        FOR(i1,w_in.rows)                          
						{
								FOR(j1,w_in.columns)
								{
								      w_in.matrix[i1][j1] = w_in.matrix[i1][j1] + z2*s.matrix[i1][j1];		
								}
						}
						F2=obj_fun(w_in,w1,Y,Y_rows,C,C2,S,X,Xu,R);
						M = M - 1; 
						i = (length<0)?i+1:i;        
					    D2=Multiply_Matrices(Transpose(F2.mat.matrix,F2.mat.rows,F2.mat.columns),s.matrix,F2.mat.columns,s.columns,s.rows); 
					    d2=D2[0][0];
			    }
	if(success)
    {
    	// if line search succeeded
         F.obj = F2.obj;
		 FX++;
         DD d_f,d_s;
         DD** tt=Multiply_Matrices(Transpose(F2.mat.matrix,F2.mat.rows,F2.mat.columns),F2.mat.matrix,F2.mat.columns,F2.mat.columns,F2.mat.rows); 
         d_f=tt[0][0];
         tt=Multiply_Matrices(Transpose(F.mat.matrix,F.mat.rows,F.mat.columns),F2.mat.matrix,F.mat.columns,F2.mat.columns,F2.mat.rows);
		 d_f= d_f-tt[0][0];
		 tt=Multiply_Matrices(Transpose(F.mat.matrix,F.mat.rows,F.mat.columns),F.mat.matrix,F.mat.columns,F.mat.columns,F.mat.rows);
		 d_s=tt[0][0];//Polack-Ribiere direction
		 FOR(i1,s.rows)
		 {
		 	FOR(j1,s.columns)
		 	{
		 		s.matrix[i1][j1]=(d_f/d_s)*s.matrix[i1][j1]-F2.mat.matrix[i1][j1];
		 	}
		 }
         temp = F.mat.matrix; //swap derivatives
		 F.mat.matrix=F2.mat.matrix;
		 F2.mat.matrix= temp;                 
		 D2=Multiply_Matrices(Transpose(F.mat.matrix,F.mat.rows,F.mat.columns),s.matrix,F.mat.columns,s.columns,s.rows); 
         d2 = D2[0][0];
         if(d2 > 0)//new slope must be negative                             
          {
            s.matrix = copy(F.mat.matrix,F.mat.rows,F.mat.columns,-1); 
            D2=Multiply_Matrices(Transpose(s.matrix,s.rows,s.columns),s.matrix,s.columns,s.columns,s.rows); 
            d2 = -1*D2[0][0];//otherwise use steepest direction
          }
         z1 = z1 * min(RATIO, d1/(d2-pow(10,-320)));d1 = d2;is_failed = 0;                              
         DD max_val=s.matrix[0][0];
         FOR(i1,s.rows)
         FOR(j1,s.columns)
         {
         	if(abs(s.matrix[i1][j1])>max_val)max_val=abs(s.matrix[i1][j1]);
         }
         if ((max_val< opt2.tolX) | (-d2 < opt2.tolFun))break;// we are at the minimum
    }
    else
    {
    	//restore point from before failed line search
         w_in.matrix = X0; 
		 F.obj = f0; 
		 F.mat.matrix = df0;  
         if(is_failed | i > abs(length))break;                        
         temp = F.mat.matrix; 
		 F.mat.matrix=F2.mat.matrix;
		 F2.mat.matrix= temp;          
         s.matrix=copy(F.mat.matrix,F.mat.rows,F.mat.columns,-1);           
         D1=Multiply_Matrices(Transpose(s.matrix,s.rows,s.columns),s.matrix,s.columns,s.columns,s.rows); 
         d1 = -1*D1[0][0];
         z1 = 1/(1-d1);                     
         is_failed = 1;   
    }
    }
    ret_mini.count=i;
    ret_mini.FX=FX;
    return ret_mini;
}
