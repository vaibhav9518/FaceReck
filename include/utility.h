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
// function returning the zero matrix with dimension RowsXCols
DD** Zeros(LL Rows, LL Cols)
{
    LL i=0;
	DD** Return_mat=(DD**)calloc(Rows,sizeof(DD*));
	FOR(i,Rows)
	    Return_mat[i]=(DD*)calloc(Cols,sizeof(DD));	
	return Return_mat;;    
}
// Function to return a matrix same as the input matrix
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
//Function to return norm of a vector
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
