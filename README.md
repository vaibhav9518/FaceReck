# FaceReck
### Google/PSU Summer of Code 2015 project:
###### Face Detection and Recognition in Videos (using semi-supervised learning based Algorithms)

* **Mentoring Organization**: Portland State University
* **Mentor**: Bart Massey
* **Student**: Vaibhav Sharma

## Overview
FaceReck is a software tool built using C, C++ and Python, which can be used to recognize faces of celebrities
in videos. The tool uses semi-supervised learning which is a class of supervised learning that also makes use of unlabeled data for training.
The tool has two main algorithms implemented:
* **Modified Adsorption** - http://talukdar.net/papers/adsorption_ecml09.pdf
* **Low Density Separation** -http://www.gatsby.ucl.ac.uk/aistats/fullpapers/198.pdf

## Prerequisites
The following libraries should be installed before installing this software:
* **Python libraries**
    * Scipy
    * Numpy
    * scikit-learn
    * python-opencv
* **C++ Libraries**
    * GSL (GNU scientific library, version 1.7 or higher )   
    * Armadillo


## Installation
From linux terminal, navigate to the directory `FaceReck-master` and run the shell script `install.sh`.
```
$ cd path/to/FaceReck-master
$ sh install.sh
```
The file 'install.sh' will create a new directory and will compile the source files present in the src directory. 
For manual compilation type 'make help' in terminal.
```
$ make help
```
Three executables will be created in the `bin` folder after the installation process is complete.


## Usage

##### For creating training data
The `Training_data` directory contains two files, an XML file for the training the face detection system and another `make_data.py`Python file, which can be used to generate new training data files from images. These data files can be 
passed as arguments to the software tool for training it to recognize new celebrities.

The make_data.py file takes in as arguments(paths to folders) that contain images of celebrities. The folder should contain atleast 300 images of each celebrity and each image should have size 50X50 pixels.
```
$ python path/to/make_data.py path/to/folder1 path/to/folder2
```
This command will execute the python script and will create two `.txt` files in the current directory(`data1.txt` and `data2.txt`). These files contain features of dimension 2500, extracted from the images present in the folders passed as arguments.

##### For running the tool
The bin folder contains a python file FaceReck which can be executed by typing `FaceReck` in the terminal. This file creates a PyQT based GUI that has 3 buttons: `Start`,`Stop` and `Test`. The video is displayed in the GUI and upon pressing `Test` the Face detection system starts cropping faces from the current frame. All the images generated are saved in the `Images` folder.
The output can be check through the `results` file or in the text-box displayed with the video and compare it with the images generated. Also, information like the output of supervised classifier, current image being cropped and total faces detected
is shown as output on the terminal.

FaceReck takes 4 arguments:
  * **Path/To/video** - The faces of celebrities present in this video will be used to make testing data for the classifier.
  * **Algorithm** - You can specify which classification algorithm you want to use (LDS/MAD).
  * **Path/To/Training_data1** - This is the file that contains features of first training class.
  * **Path/To/Training_data1** - This is the file that contains features of second training class.

The `Examples` directory has two example shell scripts. These files require training data and videos, here is the link to download Example_data: https://drive.google.com/file/d/0B9i0fnOFMkRvd1RRVzhXWUE1VkU/view?usp=sharing

You can also run the tool using example data and change the second argument as per the algorithm requirement.
```
$ FaceReck Example_data/Video/Zoobi_doobi.mp4 LDS Example_data/amir_data.txt Example_data/kareena_data.txt
```

