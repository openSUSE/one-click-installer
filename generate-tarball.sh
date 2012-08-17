#! /bin/bash

version=$( egrep -o '([0-9]+\.)+[0-9]+' version.h )
foldername=oneclickinstaller-$version
echo $version
echo $foldername

tempfolder=$( mktemp -d /tmp/tmp.XXXX ) || { echo "mktemp failed" >&2 ; exit 1 ; }
srcfolder=$( pwd )
echo $tempfolder

mkdir -p $tempfolder/$foldername/src/res

cp src/*.h src/*.cpp src/CMakeLists.txt src/oci.desktop $tempfolder/$foldername/src
cp src/res/oneclickinstall.png $tempfolder/$foldername/src/res

branch=$( git branch | grep -o tags )
echo $branch

git tag -a v$version -m 'version '.$version
git push --tags

cd $tempfolder
tar -cvf oneclickinstaller-$version.tar.gz oneclickinstaller-$version
cp oneclickinstaller-$version.tar.gz $srcfolder
rm -rf $tempfolder/$foldername
