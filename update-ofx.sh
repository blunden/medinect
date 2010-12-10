#!/bin/sh

echo "Update ofxKinect from github"
cd ../ofxKinect/
git pull
cp src/ofxKinect/src/ofxKinect.cpp ../medinect/src/ofxKinect/src/
cp src/ofxKinect/src/ofxKinect.h ../medinect/src/ofxKinect/src/
cd ..
echo "Done"
