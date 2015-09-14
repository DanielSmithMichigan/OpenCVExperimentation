#!/usr/bin/env bash
cd ~
wget https://github.com/Itseez/opencv/archive/3.0.0.zip
unzip 3.0.0.zip
mv opencv-3.0.0 opencv-install
cd opencv-install
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE ../
make -j8
make install