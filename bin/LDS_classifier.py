import numpy as np
import sys,os
import gc
from sklearn.multiclass import OutputCodeClassifier,OneVsRestClassifier,OneVsOneClassifier
from sklearn.svm import LinearSVC
from sklearn.neighbors.nearest_centroid import NearestCentroid
import sklearn.svm as svm
from sklearn import decomposition
gc.collect()
number_of_test_vectors=299
confident_vectors=10
#os.system("g++ ../src/Source.cpp -o LDS -I ../include -L/usr/local/lib -DARMA_DONT_USE_WRAPPER -lblas -llapack -lgsl -lgslcblas -lm") 
#print sys.argv[1]
def check_accuracy(Y,number,classes):
    correct=0.0
    Y=list(Y)
    print Y
    for i in range(classes):
        for j in range(number):
             if(Y[i*number+j]==i):
                   correct+=1.0
    return correct/len(Y)
def check_accuracy2(Y,number,classes):
    correct=0.0
    Y=list(Y)
    for i in range(classes):
        for j in range(number[i]):
             if(Y[i*number[i-1]+j]==i):
                   correct+=1.0
    return correct/len(Y)

X=[]
dimension=2500
print sys.argv[2],sys.argv[3]
raw_input("check")
X=X+list(np.fromfile(sys.argv[2],float,2500*300," "))
X=X+list(np.fromfile(sys.argv[3],float,2500*300," "))
pca=decomposition.KernelPCA(kernel='poly',degree=2)
X=np.array(X)
#X=X.reshape(381,2500)
X=X.reshape(600,dimension)
pca.fit(list(X))
X=np.array(pca.transform(list(X)))
print X.shape
Test_number=int(sys.argv[1])
Pre=np.fromfile("vectors.txt",float,dimension*Test_number,"  ")
#Y=[0]*234+[1]*147
Y=[0]*300+[1]*300
print len(Y)
Pre=Pre.reshape(Test_number,dimension)
dimension=len(X[0])
Pre=np.array(pca.transform(list(Pre)))
#os.remove('vectors.txt')
lin_clf = svm.LinearSVC()
lin_clf.fit(X, Y) 
result=lin_clf.predict(Pre)
print "Supervised Classifier Result"
for i in range(len(result)):
	print i,result[i]
number_of_test_vectors=Test_number

a=lin_clf.decision_function(Pre)
two_D=[]
for i in range(number_of_test_vectors):
    two_D+=[[a[i],i]]
two_D=sorted(two_D,key=lambda x:x[0])
best_vectors=[]
F1_lis=[]
F2_lis=[]
for i in range(confident_vectors):
    best_vectors=[two_D[i]]+best_vectors
    F1_lis+=[two_D[i][1]]
    best_vectors=best_vectors+[two_D[-i-1]]
    F2_lis+=[two_D[-i-1][1]]
final_labels=["" for x in range(len(Pre))]
f1=open("../data/final_file_train.txt","w")
f2=open("../data/final_file_test.txt","w")
for i in F1_lis:
    final_labels[i]=1
    for j in Pre[i]:
       f1.write(str(j)+" ")
for i in F2_lis:
    final_labels[i]=2
    for j in Pre[i]:
       f1.write(str(j)+" ")
F1_lis+=F2_lis
for i in range(len(Pre)):
       for j in Pre[i]:
        f2.write(str(j)+" ")
f1.close()
f2.close()            
print "\n---------Confident vectors generated--------\n"
os.system("./LDS "+str(len(Pre))+" "+str(2*confident_vectors)+" "+str(dimension))
#os.remove("LDS")
file=open("../results.txt","r")
result=[int(float(x)) for x in file.read().split()]
file.close()
file=open("../results.txt","w")
k=0 
for m in range(len(Pre)):
    if(final_labels[m]==""):
        final_labels[m]=result[k]
        k+=1
    if(final_labels[m]==1):
       file.write("\nImage number %d is of salman\n"%m)    
    elif(final_labels[m]==2):
       file.write("\nImage number %d is of madhuri\n"%m)       
    else:
       file.write("\nImage number %d is of others\n"%m)          
file.close()