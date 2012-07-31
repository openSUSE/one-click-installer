#! /bin/bash

version=$( grep -o [0-9].[0-9] VERSION.txt )
foldername=oneclickinstaller-$version
echo $version
echo $foldername

rm -rf $foldername

mkdir -p $foldername/src/res

cp src/*.h src/*.cpp src/CMakeLists.txt src/oci.desktop $foldername/src
cp src/res/oneclickinstall.png $foldername/src/res

tar -cvf oneclickinstaller-$version.tar.gz oneclickinstaller-$version
