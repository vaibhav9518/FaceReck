import numpy as np
import sys,os
import gc
from sklearn.multiclass import OutputCodeClassifier,OneVsRestClassifier,OneVsOneClassifier
from sklearn.svm import LinearSVC
from sklearn.neighbors.nearest_centroid import NearestCentroid
import sklearn.svm as svm
gc.collect()
number_of_test_vectors=299
confident_vectors=20

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


X=list(np.fromfile("Training_Data/amir2.txt",float,2500*234," "))
X=X+list(np.fromfile("Training_Data/Kareena11.txt",float,2500*65," "))
X=X+list(np.fromfile("Training_Data/Kareena12.txt",float,2500*15," "))
X=X+list(np.fromfile("Training_Data/Kareena13.txt",float,2500*67," "))
X=np.array(X)
X=X.reshape(381,2500)


Test_number=int(sys.argv[1])
Pre=np.fromfile("vectors.txt",float,2500*Test_number,"  ")
Pre=Pre.reshape(Test_number,2500)

Y=[0]*234+[1]*147
lin_clf = svm.LinearSVC()
lin_clf.fit(X, Y) 
result=lin_clf.predict(Pre)
number_of_test_vectors=Test_number

a=lin_clf.decision_function(Pre)

two_D=[]
for i in range(number_of_test_vectors):
    two_D+=[[a[i],i]]
two_D=sorted(two_D,key=lambda x:x[0])
f1=open("1.txt","w")
f2=open("2.txt","w")
for i in range(confident_vectors):
    f1.write(str(two_D[i][1])+"\n")
    f2.write(str(two_D[-i-1][1])+"\n")
f1.close()
f2.close()
print "\n---------Confident vectors generated--------\n"
os.system("./mad "+str(Test_number))
os.remove("1.txt")
os.remove("2.txt")
os.remove("vectors.txt")
os.remove("mad")

