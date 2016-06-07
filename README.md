One Click Installer
===================

One Click Install makes it easy for users to install software, no matter where that software is located. 

Prerequisite Packages
=====================
To build and develop One Click Installer, several packages may be required from your distribution. Here's a list.
Required Dependencies
----------------------
1. Qt Framework > 5.2.0
2. Libzypp API

Setting Up The Environment
--------------------------

	$ zypper install PackageKit-Qt5-devel libzypp-devel libqt5-qtbase-devel

Getting the Source Code
=======================

To get the source code, clone the GitHub repository:

	$ git clone https://github.com/openSUSE/one-click-installer.git
	
If you want to contribute into the project you can
[fork](https://help.github.com/articles/fork-a-repo/) the repository and clone your fork.

Steps to Build OCI
===================

1) Get the source code form GitHub repository

2) Change in to one-click-installer directory:

	$ cd one-click-installer
	
3) Create and change in to a build directory for compilation:

	$ mkdir build; cd build
	
4) Run and configure process with cmake:

	$ cmake -DCMAKE_INSTALL_PREFIX=/usr ..
	
5) Compile, and Install One Click Installer:

	$ make && [sudo] make install

Contributors
============

Saurabh Sood<br>
Cornelius Schumacher<br>
Matt Barringer<br>
Antonio Larrosa<br>
Shalom Rachapudi David
