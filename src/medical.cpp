#include "medical.h"

void medical::setup()
{
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();

	gestureImage.allocate(kinect.width, kinect.height);
	gestureThresh.allocate(kinect.width, kinect.height);
	gestureThreshFar.allocate(kinect.width, kinect.height);

//	activationImage.allocate(kinect.width, kinect.height);
//	activationThresh.allocate(kinect.width, kinect.height);
//	activationThreshFar.allocate(kinect.width, kinect.height);
	
	farGestureThreshold = 150;	// The threshold for general gesture activation
	nearGestureThreshold = 130;

//	farActivationThreshold = 129;	// The threshold for the activation gestures
//	nearActivationThreshold = 120;
	
	imageDisplayNumber = 0;
	
	ofSetFrameRate(24);

	verdana.loadFont("verdana.ttf", 14);
	
	startX = 0;
	startY = 0;
//	lastX = 0;
//	lastY = 0;

	inYMode = false;
}

void medical::update()
{
	ofBackground(100, 100, 100);
	kinect.update();

	gestureImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
	gestureImage.mirror(false, true);
	
	gestureThreshFar = gestureImage;
	gestureThresh = gestureImage;
	gestureThreshFar.threshold(farGestureThreshold, true);
	gestureThresh.threshold(nearGestureThreshold);
	cvAnd(gestureThresh.getCvImage(), gestureThreshFar.getCvImage(), gestureImage.getCvImage(), NULL);
	
	gestureImage.flagImageChanged();
	
	gestureFinder.findContours(gestureImage, 10, 28500, 27500, true);

	// --------------------

//	activationImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
//	activationImage.mirror(false, true),
	
//	activationThreshFar = activationImage;
//	activationThresh = activationImage;
//	activationThreshFar.threshold(farActivationThreshold, true);
//	activationThresh.threshold(nearActivationThreshold);
//	cvAnd(activationThresh.getCvImage(), activationThreshFar.getCvImage(), activationImage.getCvImage(), NULL);

//	activationImage.flagImageChanged();

//	activationFinder.findContours(activationImage, 10, 28500, 27500, true);
}

void medical::draw()
{
	ofSetColor(255, 255, 255);
//	kinect.drawDepth(10, 10, 400, 300);
//	kinect.draw(420, 10, 400, 300);
	
	if (gestureFinder.blobs.size() >= 1)
	{
		ofSetColor(0, 0, 255);
		ofFill();
		ofxCvBlob &blob = gestureFinder.blobs[0];
		int x = blob.boundingRect.x;
		int y = blob.boundingRect.y;
		verdana.drawString("X: " + ofToString(x, 2) + ", Y: " + ofToString(y, 2), 20, 600);
//		verdana.drawString("startX: " + ofToString(startX, 2) + ", startY: " + ofToString(startY, 2), 700, 650);
		if ((y <= (startY + 40)) && (y >= (startY - 40)) 
		    && (x <= (startX + 200)) && !inYMode) // They are still in the hover corridor
		{
			verdana.drawString("You are in the corridor!", 20, 500);
			ofSetColor(0, 255, 255);
			if (x >= (startX + 120))
			{
//				verdana.drawString("Entering YMode!", 700, 500);
				inYMode = true;
				startX = x;
			}
		}
		else if ((x <= (startX + 40)) && (x >= (startX - 40))
			 && (y >= (startY - 150)) && inYMode) // They are in the y-hover corridor
		{
			verdana.drawString("In YMode!", 20, 500);
			ofSetColor(255, 255, 0);
			if (y <= (startY - 80))
			{
//				verdana.drawString("Activate the Widget by pushing forward!", 700, 600);
				ofSetColor(100, 100, 100);
				inActivationMode = true;
				lastX = x;
				lastY = y;
			}
		}
		else if (inActivationMode && (gestureFinder.blobs.size() == 1)) // they are in the activation mode
		{
			int aX = gestureFinder.blobs[0].boundingRect.x; // activationFinder, changed for performance testing
			int aY = gestureFinder.blobs[0].boundingRect.y;
			verdana.drawString("You have FUCKED THE WIDGET!", 20, 500);
			ofSetColor(107, 142, 35);
			// We just need to ensure that the activation object is in roughly the same spot as the gesture object
			if ((aX <= lastX + 100) && (aX >= lastX - 100) && (aY <= lastY + 100) && (aY >= lastY - 100))
			{
				ofSetColor(0, 255, 0);
//				verdana.drawString("You have Activated the Widget!", 700, 600);
			}
		}
		else // They exited the hover corridor
		{
			verdana.drawString("Exitting the corridor!", 20, 500);
			ofSetColor(255, 0, 0);
			startY = y;
			startX = x;
			inYMode = false;
			inActivationMode = false;
		}
		ofCircle(x, y, 10);
	}
	else
	{
		verdana.drawString("I Cannot See Your Hand!", 20, 20);
	}
	
	if (inActivationMode && (gestureFinder.blobs.size() == 1))
	{
		int aX = gestureFinder.blobs[0].boundingRect.x; // activationFinder, changed for performance testing
		int aY = gestureFinder.blobs[0].boundingRect.y;
		int d = 100;
		ofSetColor(107, 142, 35);
		if ((aX <= lastX + d) && (aX >= lastX - d) && (aY <= lastY + d) && (aY >= lastY - d))
		{
			ofSetColor(0, 255, 0);
			verdana.drawString("You have Activated the Widget!", 20, 500);
		}
		ofCircle(aX, aY, 10);
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

	ofNoFill();
	ofSetColor(255, 255, 255);
	
//	gestureImage.draw(700, 20, 400, 300);
//	activationImage.draw(700, 320, 400, 300);
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
