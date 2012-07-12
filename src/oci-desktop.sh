#!/bin/sh
cat <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=One Click Installer
GenericName=Beautiful One Click Installer
Comment=A Tool to Install Software with a single click
TryExec=$1/bin/oneclickinstaller
Exec=$1/bin/oneclickinstaller &
Categories=Application;System;Configuration;
Icon=$1/share/icons/oneclickinstaller.png
EOF