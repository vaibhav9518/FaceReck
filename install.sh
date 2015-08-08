make -f  $(dirname $0)/makefile all
SCRIPTPATH=$( cd $(dirname $0) ; pwd -P )
sudo ln -s  $SCRIPTPATH/bin/FaceReck /usr/bin/FaceReck
chmod +x bin/FaceReck
chmod +x bin/MAD
chmod +x bin/LDS
