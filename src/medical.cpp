#include "medical.h"

void medical::setup()
{
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();

	bool loadImage1 = image1.loadImage("test1.jpg");
	bool loadImage2 = image2.loadImage("test2.jpg");
	
	grayImage.allocate(kinect.width, kinect.height);
	grayThresh.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	farThreshold = 150;
	nearThreshold = 120;
	
	imageDisplayNumber = 0;
	
	ofSetFrameRate(40);
}

void medical::update()
{
	ofBackground(100, 100, 100);
	kinect.update();

	grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
	
	grayThreshFar = grayImage;
	grayThresh = grayImage;
	grayThreshFar.threshold(farThreshold, true);
	grayThresh.threshold(nearThreshold);
	cvAnd(grayThresh.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
	
	grayImage.flagImageChanged();
		
//	cout << "Kinect width: " << kinect.width << ", Kinect height: " << kinect.height << "\n";	

	contourFinder.findContours(grayImage, 10, 28500, 27500, true);
}

void medical::draw()
{
	ofSetColor(255, 255, 255);
//	kinect.drawDepth(10, 10, 400, 300);
//	kinect.draw(420, 10, 400, 300);
	
	if (contourFinder.blobs.size() >= 1)
	{
		ofxCvBlob &blob = contourFinder.blobs[0];
		ofCircle(blob.boundingRect.x, blob.boundingRect.y, 50);
	}
	
	switch (imageDisplayNumber)
	{
		case 1:
			image1.draw(10, 320, 400, 300);
			break;
		case 2:
			image2.draw(10, 320, 400, 300);
			break;
		default:
			break;
	}
	
	grayImage.draw(500, 20, 400, 300);
//	contourFinder.draw(420, 320, 400, 300);
}

void medical::exit()
{
	kinect.close();
}

void medical::windowResized(int w, int h)
{
	
}

void medical::keyPressed(int key)
{
	cout << key << " has been pressed.\n";
	switch (key)
	{
		case 49:
			imageDisplayNumber = 1;
			break;
		case 50:
			imageDisplayNumber = 2;
			break;
		default:
			break;
	}
	cout << "Image Display Number is now: " << imageDisplayNumber << "\n";
}
