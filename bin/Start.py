import cv
import sys,os
 
from PyQt4.QtCore import QPoint, QTimer,Qt
from PyQt4.QtGui import QApplication, QImage, QPainter, QWidget,QPushButton,QTextEdit,QMenuBar
import threading 
from PyQt4.QtCore import *
from PyQt4.QtGui import *
 
class IplQImage(QImage):
 
    def __init__(self,iplimage):
        # Rough-n-ready but it works dammit
        alpha = cv.CreateMat(iplimage.height,iplimage.width, cv.CV_8UC1)
        cv.Rectangle(alpha, (0, 0), (iplimage.width,iplimage.height), cv.ScalarAll(255) ,-1)
        rgba = cv.CreateMat(iplimage.height, iplimage.width, cv.CV_8UC4)
        cv.Set(rgba, (1, 2, 3, 4))
        cv.MixChannels([iplimage, alpha],[rgba], [
        (0, 0), # rgba[0] -> bgr[2]
        (1, 1), # rgba[1] -> bgr[1]
        (2, 2), # rgba[2] -> bgr[0]
        (3, 3)  # rgba[3] -> alpha[0]
        ])
        self.__imagedata = rgba.tostring()
        super(IplQImage,self).__init__(self.__imagedata, iplimage.width, iplimage.height, QImage.Format_RGB32)
 
class myThread (threading.Thread):
    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.tex=0
        self.started=False
        self.completed=False
        self.frame=200
    def run(self):
        self.started=True
        print_time(self.tex,self)    

def print_time(tex,thread):
      #tex. 
      #for i in range(100):
      #     tex.append("hello")
      os.system("python video.py ../Training_data/haarcascade_frontalface_default.xml 200 "+sys.argv[1]+" "+str(thread.frame)+" "+sys.argv[2]+" "+sys.argv[3]+" "+sys.argv[4])
      thread.started=False
      thread.completed=True

thread1 = myThread(1, "Thread-1", 0)



class VideoWidget(QWidget):
    """ A class for rendering video coming from OpenCV """
 
    def __init__(self, parent=None):
        QWidget.__init__(self)
        layout = QVBoxLayout()
        print sys.argv[1]
        self._capture = cv.CaptureFromFile(sys.argv[1])
        # Take one frame to query height
        self.count=0
        frame = cv.QueryFrame(self._capture)
        self.setMinimumSize(1155, frame.height)
        self.setMaximumSize(self.minimumSize())
        self._frame = None
        self.button = QPushButton('Test', self)
        layout.addWidget(self.button)        
        layout.setAlignment(self.button, Qt.AlignRight|Qt.AlignTop)
        
        self.button2 = QPushButton('Start', self)
        self.button2.move(980,10)
        layout.setAlignment(self.button2, Qt.AlignRight|Qt.AlignTop)
        self.button2.setFixedSize(80,28)
        self.button2.clicked.connect(self.Start_button)

        self.button3 = QPushButton('Stop', self)
        layout.setAlignment(self.button3, Qt.AlignRight|Qt.AlignTop)
        self.button3.move(900,10)
        self.button3.setFixedSize(80,28)
        self.button3.clicked.connect(self.Stop_button)
        
        self.lod_count=0 

        self.te = QTextEdit(self)
        layout.addWidget(self.te)        
        layout.setAlignment(self.te, Qt.AlignRight)

        self.button.clicked.connect(self.handleButton)
        self._image = self._build_image(frame)
        self._timer = QTimer(self)
        self._timer.timeout.connect(self.queryFrame)
        self._timer.start(30)
        layout.setAlignment(Qt.AlignTop)
        self.setLayout(layout)
    def Start_button(self):
        self._timer.start(30)
    def Stop_button(self):
        self._timer.stop()
    def handleButton(self):
        #global thread1
        #thread1.start()
        global thread1
        thread1.tex=self.te
        thread1.frame=self.count
        thread1.start()
        #self._timer.start()
    def _build_image(self, frame):
        if not self._frame:
            self._frame = cv.CreateImage((frame.width, frame.height), cv.IPL_DEPTH_8U, frame.nChannels)
        if frame.origin == cv.IPL_ORIGIN_TL:
            cv.Copy(frame, self._frame)
        else:
            cv.Flip(frame, self._frame, 0)
        return IplQImage(self._frame)
 
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.drawImage(QPoint(0, 0), self._image)
 
    def queryFrame(self):
        global thread1
        if(not thread1.started):  
          frame = cv.QueryFrame(self._capture)  
          self.count+=1
          self._image = self._build_image(frame)
        else:
          self.lod_count+=1  
          if(self.lod_count==1):
            self.te.insertPlainText("Processing Images")
          if(self.lod_count%50==0):
            self.te.insertPlainText(" .")
          if(self.lod_count%200==0):
            self.lod_count%=200
            self.te.clear()
          #self.te.append("done")
        if(thread1.completed):
            self.te.clear()
            f=open("../results.txt")
            self.te.insertPlainText("\t--Results--\n")
            k=f.readline()
            while k!="":
                 self.te.insertPlainText(k)
                 k=f.readline()
            f.close()
            thread1.completed=False     
        self.update()   
 
if __name__ == '__main__':
    app = QApplication(sys.argv)
 
    widget = VideoWidget()
    widget.setWindowTitle('Celebrity Recognition')
    widget.show()
    
    sys.exit(app.exec_())
    if thread1.isAlive():
      thread1.join()
