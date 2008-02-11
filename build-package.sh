#!/bin/sh

APPLICATION_NAME="ksystemlog-0.4.1"

echo $APPLICATION_NAME
PACKAGE_NAME="${APPLICATION_NAME}.tar.bz2"

CURRENT_FOLDER=`pwd`

echo "Building ${APPLICATION_NAME} package..."

make clean

mkdir -p /tmp/$APPLICATION_NAME
rm -rf /tmp/$APPLICATION_NAME/*
cp -R . /tmp/$APPLICATION_NAME

rm -rf /tmp/$APPLICATION_NAME/tmp
rm -rf /tmp/$APPLICATION_NAME/build
rm -f /tmp/$APPLICATION_NAME/install_manifest.txt
rm -f /tmp/$APPLICATION_NAME/$PACKAGE_NAME

find /tmp/$APPLICATION_NAME -name .svn -exec rm -rf {} \;

for tempFile in CMakeCache.txt CMakeTmp CMakeFiles Makefile *_automoc.cpp* moc_* *.moc ui_*.h Testing cmake_install.cmake cmake_uninstall.cmake DartTestfile.txt ; do
	echo "Removing $tempFile"
	
	find /tmp/$APPLICATION_NAME -printf "%p\n" -name $tempFile -exec rm -rf {} \;
done

cd /tmp
tar cjf $CURRENT_FOLDER/$PACKAGE_NAME $APPLICATION_NAME
cd -

echo "Package built : $PACKAGE_NAME."
