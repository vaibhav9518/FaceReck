import numpy as np
import sys,os
import gc
from sklearn.multiclass import OutputCodeClassifier,OneVsRestClassifier,OneVsOneClassifier
from sklearn.svm import LinearSVC
from sklearn.neighbors.nearest_centroid import NearestCentroid
import sklearn.svm as svm
gc.collect()
number_of_test_vectors=299
confident_vectors=10

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
X=X+list(np.fromfile("Training_data/amir2.txt",float,2500*234," "))
X=X+list(np.fromfile("Training_data/Kareena11.txt",float,2500*65," "))
X=X+list(np.fromfile("Training_data/Kareena12.txt",float,2500*15," "))
X=X+list(np.fromfile("Training_data/Kareena13.txt",float,2500*67," "))
X=np.array(X)
X=X.reshape(381,2500)
Test_number=int(sys.argv[1])
Pre=np.fromfile("vectors.txt",float,2500*Test_number,"  ")
Y=[0]*234+[1]*147
Pre=Pre.reshape(Test_number,2500)
os.remove('vectors.txt')
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

f1=open("data/final_file_train.txt","w")
f2=open("data/final_file_test.txt","w")
for i in F1_lis:
    for j in Pre[i]:
       f1.write(str(j)+" ")
for i in F2_lis:
    for j in Pre[i]:
       f1.write(str(j)+" ")
F1_lis+=F2_lis
for i in range(len(Pre)):
       for j in Pre[i]:
        f2.write(str(j)+" ")
f1.close()
f2.close()            
print "\n---------Confident vectors generated--------\n"
