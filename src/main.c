#include <MAD.h>
// arrange labels in descending order as in first give 0 labels then 1
// the matrix are read row wise as in each vector is a row of matrix k using it we form a weight matrix W
int main(int argc,char** argv){
    LL i,j,confident_vectors;
    FILE *file1;
    file1 = fopen("vectors.txt", "r");
    LL rows=atoi(argv[1]),columns=atoi(argv[3]);
    confident_vectors=atoi(argv[2]);
    DD** k=(DD**)calloc(rows,sizeof(DD*));
    DD** temp;
    FOR(i,rows)
    {
	    k[i]=(DD*)calloc(columns,sizeof(DD));	
	    FOR(j,columns)
	    {
	    fscanf(file1,"%lf  ",&k[i][j]);
	    }
    }
    fclose(file1);
    temp=k;
    k=generate_weights_cosine(k,rows,columns,100);
    FOR(i,rows)free(temp[i]);
    free(temp);
    DD* labels=(DD*)calloc(columns,sizeof(DD));
    FOR(i,columns)
    {
	labels[i]=-1;
    }
    file1 = fopen("1.txt", "r");
    FOR(i,confident_vectors)
    {
    fscanf(file1,"%ld\n",&j);
    labels[j]=1;
    }
    fclose(file1);
    file1 = fopen("2.txt", "r");
    FOR(i,confident_vectors)
    {
    fscanf(file1,"%ld\n",&j);
    labels[j]=0;
    }
    DD** results=MAD(k,rows,2,labels);
    file1 = fopen("results.txt", "w");
    FOR(i,rows)
    {
        if(((int)results[i][0])==0)fprintf(file1,"Image number %ld is of Kareeena\n",i+1);
        else if(((int)results[i][0])==1)fprintf(file1,"Image number %ld is of Amir\n",i+1);
        else fprintf(file1,"others\n");
    }
    fclose(file1);
    return 0;
}
