import sys,cv2,os
import numpy
import numpy as np
import cv2
from sklearn import cluster
cur_dir=os.path.dirname(os.path.realpath(__file__))

mypath=[sys.argv[1],sys.argv[2]]
filename=[cur_dir+'/data1.txt',cur_dir+'/data2.txt']
for ii in range(len(mypath)):
	from os import listdir
	from os.path import isfile, join
	onlyfiles = [ f for f in listdir(mypath[ii]) if isfile(join(mypath[ii],f)) ]
	file=open(filename[ii],'w')
	assert(len(onlyfiles)>=300)
	for i in onlyfiles[0:300]:
		i=mypath[ii]+'/'+i
		crop_img=cv2.imread(i)
		print "Processing",i
		if(crop_img==None):
			continue
		crop_img=cv2.resize(crop_img,(50,50),fx=0,fy=0)
		a=[[0 for y in range(50)] for x in range(50)]
		for i in range(len(crop_img)):
			b=[]
			for j in range(len(crop_img[i])):  
				a[j][i]=sum(crop_img[i][j])/3
		a=numpy.array(a)
		a=a.reshape(1,50*50)
		numpy.savetxt(file,a,fmt="%10.2f")
	file.close()	

