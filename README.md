# FaceReck
###Google/PSU Summer of Code 2015 project:
######Face Detection and Recognition in Videos (using semi-supervised learning based Algorithms)

* **Mentoring Organization**: Portland State University
* **Mentor**: Bart Massey
* **Student**: Vaibhav Sharma


###Prerequisites
The following libraries should be installed before installing this software:
* **Python libraries**
    * Scipy
    * Numpy
    * scikit-learn
    * python-opencv
* **C++ Libraries**
    * GSL (GNU scientific library, version 1.7 or higher )   
    * Armadillo


### Installation
From linux terminal navigate to the directory FaceReck-master and run the shell script 'install.sh'.
```
$ cd path/to/FaceReck-master
$ sh install.sh
```
The file 'install.sh' will create a new directory and will compile the source files present in the src directory,for
manual compilation type 'make help' in terminal.
```
$ make help
```
Three executables will be created in the 'bin' folder after this.
