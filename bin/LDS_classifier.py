import numpy as np
import sys,os
import gc
from sklearn.multiclass import OutputCodeClassifier,OneVsRestClassifier,OneVsOneClassifier
from sklearn.svm import LinearSVC
from sklearn.neighbors.nearest_centroid import NearestCentroid
import sklearn.svm as svm
from sklearn import decomposition
"""
    --> This file classifies the Faces detected by Face detection system
    --> and applies supervised classifier to it.It also creates training
    --> and testing files for the next 'LDS' algorithm based semi-supervised
    --> classifier.The initial supervised classifier is sklearn.svm.LinearSVC
    --> and the selected confident vectors are passed to the binary file LDS
    --> along with testing.
"""
gc.collect()
number_of_test_vectors=299
confident_vectors=10# No. of confident selected as Labels for Supervised classifier
cur_dir=os.path.dirname(os.path.realpath(__file__))# get path of current directory
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
dimension=2500#
print sys.argv[2],sys.argv[3]

#          ************ Load Training Data *************            

X=X+list(np.fromfile(sys.argv[2],float,2500*300," "))# Load 300 vectors of each class
X=X+list(np.fromfile(sys.argv[3],float,2500*300," "))
pca=decomposition.KernelPCA(kernel='poly',degree=2)# use kernelPCA instead of PCA
X=np.array(X)
X=X.reshape(600,dimension)
pca.fit(list(X))
X=np.array(pca.transform(list(X)))
print X.shape
Test_number=int(sys.argv[1])

#          *********** Load Testing Data ***************            

Pre=np.fromfile(cur_dir+"/vectors.txt",float,dimension*Test_number,"  ")
Y=[0]*300+[1]*300# set Labels for training vectors
print len(Y)
Pre=Pre.reshape(Test_number,dimension)
dimension=len(X[0])# get new dimension after applying KPCA
Pre=np.array(pca.transform(list(Pre)))
lin_clf = svm.LinearSVC()
lin_clf.fit(X, Y) 
result=lin_clf.predict(Pre)
print "Supervised Classifier Result"
for i in range(len(result)):
	print i,result[i]
number_of_test_vectors=Test_number

a=lin_clf.decision_function(Pre)# get decision function values to select most confident 
two_D=[]
for i in range(number_of_test_vectors):
    two_D+=[[a[i],i]]
two_D=sorted(two_D,key=lambda x:x[0])
best_vectors=[]
F1_lis=[]
F2_lis=[]

#      ******* Select Most confident vectors for semi-supervised classifier Labels *********

for i in range(confident_vectors):
    best_vectors=[two_D[i]]+best_vectors
    F1_lis+=[two_D[i][1]]
    best_vectors=best_vectors+[two_D[-i-1]]
    F2_lis+=[two_D[-i-1][1]]
final_labels=["" for x in range(len(Pre))]
f1=open(cur_dir+"/../data/final_file_train.txt","w")
f2=open(cur_dir+"/../data/final_file_test.txt","w")
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

os.system(cur_dir+"/LDS "+str(len(Pre))+" "+str(2*confident_vectors)+" "+str(dimension)+" "+cur_dir)
file=open(cur_dir+"/../results.txt","r")
result=[int(float(x)) for x in file.read().split()]
file.close()
file=open(cur_dir+"/../results.txt","w")
k=0 
for m in range(len(Pre)):
    if(final_labels[m]==""):
        final_labels[m]=result[k]
        k+=1
    if(final_labels[m]==1):
       file.write("\nImage number %d is of class 1\n"%m)    
    elif(final_labels[m]==2):
       file.write("\nImage number %d is of class 2\n"%m)       
    else:
       file.write("\nImage number %d is of others\n"%m)          
file.close()
os.remove(cur_dir+"/vectors.txt")
