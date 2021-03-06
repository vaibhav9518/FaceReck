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
    --> and testing files for the next 'MAD' algorithm based semi-supervised
    --> classifier.The initial supervised classifier is sklearn.svm.LinearSVC
    --> and the selected confident vectors are passed to the binary file MAD
    --> along with testing.
"""
gc.collect()
confident_vectors=20# No. of confident selected as Labels for Supervised classifier
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

#           *********** Load Training Data **********            

X=list(np.fromfile(sys.argv[2],float,2500*300," "))# Load 300 vectors of each class
X=X+list(np.fromfile(sys.argv[3],float,2500*300," "))

X=np.array(X)
dimension=2500
X=X.reshape(600,dimension)
pca=decomposition.KernelPCA(kernel='poly',degree=3)# use KernelPCA instead of PCA
pca.fit(list(X))
X=np.array(pca.transform(list(X)))

#           *********** Load Testing Data **********

Test_number=int(sys.argv[1])
Pre=np.fromfile(cur_dir+"/vectors.txt",float,2500*Test_number,"  ")
Pre=Pre.reshape(Test_number,dimension)
dimension=len(X[0])# get new dimension after applying KPCA
Pre=np.array(pca.transform(list(Pre)))

Y=[0]*300+[1]*300#set Labels for training vectors
lin_clf = svm.LinearSVC()
lin_clf.fit(X, Y) 
result=lin_clf.predict(Pre)
number_of_test_vectors=Test_number

a=lin_clf.decision_function(Pre)# get decision function values to select most confident 

two_D=[]
for i in range(number_of_test_vectors):
    two_D+=[[a[i],i]]
two_D=sorted(two_D,key=lambda x:x[0])
f1=open(cur_dir+"/1.txt","w")
f2=open(cur_dir+"/2.txt","w")

#    ******* Select Most confident vectors for semi-supervised classifier Labels ********           

for i in range(confident_vectors):
    f1.write(str(two_D[i][1])+"\n")
    f2.write(str(two_D[-i-1][1])+"\n")
f1.close()
f2.close()
print "\n---------Confident vectors generated--------\n"
os.system(cur_dir+"/MAD "+str(Test_number)+" "+str(confident_vectors)+" "+str(dimension)+" "+cur_dir)
print "MAD Done"
os.remove(cur_dir+"/1.txt")
os.remove(cur_dir+"/2.txt")
os.remove(cur_dir+"/vectors.txt")

