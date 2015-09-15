# Copied from http://www.pyimagesearch.com/2015/06/22/install-opencv-3-0-and-python-2-7-on-ubuntu/
sudo apt-get --force-yes -y update
sudo apt-get --force-yes -y upgrade
sudo apt-get --force-yes -y install build-essential cmake git pkg-config
sudo apt-get --force-yes -y install libjpeg8-dev libtiff4-dev libjasper-dev libpng12-dev
sudo apt-get --force-yes -y install libgtk2.0-dev
sudo apt-get --force-yes -y install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get --force-yes -y install libatlas-base-dev gfortran
cd ~
mkdir opencv-install
cd opencv-install
git clone https://github.com/Itseez/opencv.git
cd opencv
git checkout 3.0.0
mkdir build
cd build
cmake   -D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=/usr/local \
	-D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules ..
make -j8
sudo make install
sudo ldconfig
