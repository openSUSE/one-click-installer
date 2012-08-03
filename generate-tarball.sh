#! /bin/bash

version=$( egrep -o '([0-9]+\.)+[0-9]+' VERSION.txt )
foldername=oneclickinstaller-$version
echo $version
echo $foldername

mkdir -p /tmp/$foldername/src/res

cp src/*.h src/*.cpp src/CMakeLists.txt src/oci.desktop /tmp/$foldername/src
cp src/res/oneclickinstall.png /tmp/$foldername/src/res

branch=$( git branch | grep -o tags )
echo $branch

if [ $branch == "tags" ]; then
git tag -a v$version -m 'version '.$version
git push -u origin
else
	echo "Not in tags. Not pushed"
fi

cd /tmp
tar -cvf oneclickinstaller-$version.tar.gz oneclickinstaller-$version

rm -rf /tmp/$foldername
