import numpy as np
import cv2,sys,os
import numpy
import threading
cur_dir=os.path.dirname(os.path.realpath(__file__))
class FaceDetect:

   def __init__(self,video,numFrames,File,faceCascade):
        self.video=video
        self.number_of_frames=numFrames  
        self.writeTo=File
        self.faceCascade=faceCascade

   def crop(self,img,x,y,w,h,N):
       crop_img = img[y:y+h,x:x+w] 
       crop_img=cv2.resize(crop_img,(50,50),fx=0,fy=0)
       gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY) 
       if(np.max(cv2.convertScaleAbs(cv2.Laplacian(gray,3)))<150):
        return False
       print "Cropping image number",N 
       a=[]
       for i in crop_img:
           b=[]
           for j in i:
              b+=[sum(j)/3]
           a+=[b]
       a=numpy.array(a)
       a=a.reshape(1,50*50)
       numpy.savetxt(self.File,a,fmt="%10.2f")
       cv2.imwrite(cur_dir+"/../Images/file"+str(N)+".jpg",crop_img)
       return True
   
   def detect(self,frame):
       faces = self.faceCascade.detectMultiScale(
            frame,
            scaleFactor=1.1,
            minNeighbors=5,
            minSize=(30, 30),
            flags=cv2.cv.CV_HAAR_SCALE_IMAGE
            )
       return faces 

   def saviour(self,vid,frame,LDSorMAD,data1,data2):
       self.File=open(self.writeTo,"w")
       cap = cv2.VideoCapture(vid)
       count=cap.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT)
       m1=max(0,frame-int(self.number_of_frames/2))     
       cap.set(cv2.cv.CV_CAP_PROP_POS_FRAMES,m1)
       val=0
       while(cap.isOpened()):
          ret, frame = cap.read()
          if(ret==False or val>=self.number_of_frames):
            break
    
          gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
          faces=self.detect(gray)

          for (x, y, w, h) in faces:
            val+=1 
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            if( not self.crop(frame,x,y,w,h,val)):
                val-=1 

          if cv2.waitKey(1) & 0xFF == ord('q'):
             break  

       print "Done" 
       print "Total Images:",val
       self.File.close()
       if(LDSorMAD=='MAD'):
          os.system("python "+cur_dir+"/MAD_classifier.py "+str(val)+" "+data1+" "+data2)
       else:
          os.system("python "+cur_dir+"/LDS_classifier.py "+str(val)+" "+data1+" "+data2)   
       cap.release()

   def other(self):
       val=0
       cap2= cv2.VideoCapture(self.video)
       while(cap2.isOpened()):
         ret, frame = cap2.read()
         if(ret==False):
           break
         val+=1  
         cv2.imshow('frame',frame)
         if cv2.waitKey(10) & 0xFF == ord('s'):
             self.saviour(self.video,val)
         elif cv2.waitKey(10) & 0xFF == ord('q'):
            break
         else:
            ""
if(len(sys.argv)<2):
   print "supply cascade path"
   assert(len(sys.argv)>=4)
elif(len(sys.argv)<3):
   print "supply number of frames"
   assert(len(sys.argv)>=4)
elif(len(sys.argv)<4):
   print "supply the video name"
   assert(len(sys.argv)>=4)
else:
   assert(len(sys.argv)>=4)
   cascPath = sys.argv[1]
   faceCascade = cv2.CascadeClassifier(cascPath)
   File=cur_dir+"/vectors.txt"
   numframes=int(sys.argv[2])
   video=sys.argv[3]
   X=FaceDetect(video,numframes,File,faceCascade)
   X.saviour(X.video,int(sys.argv[4]),sys.argv[5],sys.argv[6],sys.argv[7])
