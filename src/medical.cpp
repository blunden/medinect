#include "medical.h"
#include "osc.h"
#include <iostream>

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3
#define DIRECTION_WAIT 4

medical::medical(ofxArgs* args) 
{
	this->args = args;
}

void medical::setup()
{
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();

	gestureImage.allocate(kinect.width, kinect.height);
	gestureThresh.allocate(kinect.width, kinect.height);
	gestureThreshFar.allocate(kinect.width, kinect.height);
	
	farGestureThreshold = 130;	// The threshold for general gesture activation
	nearGestureThreshold = 100;

	imageDisplayNumber = 0;
	
	ofSetFrameRate(60);

	verdana.loadFont("verdana.ttf", 14);
	
	startX = 0;
	startY = 0;
	lastX = 0;
	lastY = 0;
	
	myTimeThen = 0.0f;
	myTimeNow = 0.0f;
	myFramerate = 0.0f;
	myFPS = 0.0f;
	myFrames = 0.0f;

	inYMode = false;
	activated = false;

	HW_FIRST_X = 0;
	HW_FIRST_Y = 0;
	HW_SPACE = 40;
	HW_DISTANCE = 150;
	HW_FIRST_LEG = -1;
	HW_SECOND_LEG = -1;
	HW_CURRENT_DIRECTION = -1;

	isActivated = false;
	ZOOM = 0.5f;
	STACK = 1;
	X_STATE = 0;
	Y_STATE = 0;

	this->dest_ip = "127.0.0.1"; //initialize to localhost
	this->dest_port = 4711; //initialize to arbitrarily big number

	if((this->args->getString(1) != "") && (this->args->getCount() == 3)) { //no verification that this is an actual ip address
		this->dest_ip = this->args->getString(1);
		this->dest_port = this->args->getInt(2);
	}
	else {
		std::cout << "Usage: " << this->args->getString(0) << " <address>" << " <port>" << endl; 
		std::cout << "Using default values" << endl;
	}
	std::cout << "Connecting to " << dest_ip << " on port " << dest_port << endl;
	sender.osc_init(dest_ip, dest_port);

	sender.send_stack(0, 0, ZOOM, STACK);
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

	gestureFinder.findContours(gestureImage, 2000, 8000, 10, true);
}

void medical::draw()
{
//	ofSetColor(0, 0, 255);
//	verdana.drawString("Framerate: " + ofToString(ofGetFrameRate()), 20, 50);
	ofDrawBitmapString(buf, 20, 50);
//	kinect.drawDepth(10, 10, 400, 300);
//	kinect.draw(420, 10, 400, 300);

	int x;
	int y;
	
	if (gestureFinder.blobs.size() == 1)
	{
//		ofSetColor(0, 0, 255);
		ofFill();
		ofxCvBlob &blob = gestureFinder.blobs[0];
		x = blob.centroid.x;
		y = blob.centroid.y;
		if (!isActivated)
			hoverWidget(x, y);
		else
		{
			if (HW_FIRST_LEG == DIRECTION_UP && HW_SECOND_LEG == DIRECTION_RIGHT)
			{
				X_STATE = x;
				Y_STATE = y;
				sender.send_stack(x, y, ZOOM, STACK);
				ofSetColor(255, 0, 255);
			}
			if (HW_FIRST_LEG == DIRECTION_DOWN && HW_SECOND_LEG == DIRECTION_LEFT)
			{
				sender.send_pen(sender.PEN_UNDO, x, y, 1, 1);
				ofSetColor(0, 255, 0);
			}
			if (HW_FIRST_LEG == DIRECTION_LEFT && HW_SECOND_LEG == DIRECTION_DOWN)
			{
				if (HW_CURRENT_DIRECTION != -1)
				{
					HW_FIRST_Y = y;
					HW_CURRENT_DIRECTION = -1;
				}
				int distance = y - HW_FIRST_Y;
//				cout << "Raw distance: " << distance << endl;
				distance = distance / 10;
//				cout << "Divided Distance: " << distance << endl;
				int speed = 0;
				if (distance < -4)
				{
//					cout << "Scroll speed -2" << endl;
					speed = -4;
				}
				else if (distance < -2)
				{
//					cout << "Scroll speed -1" << endl;
					speed = -1;
				}
				else if (distance > 4)
				{
//					cout << "Scroll speed 2" << endl;
					speed = 4;
				}
				else if (distance > 2)
				{
//					cout << "Scroll speed 1" << endl;
					speed = 1;
				}
				else
				{
//					cout << "Scroll speed 0" << endl;
					speed = 0;
				}
				STACK += speed;
				if (STACK >= 25)
					STACK = 25;
				else if (STACK <= 0)
					STACK = 0;
				cout << "STACK: " << STACK << endl;
				sender.send_stack(X_STATE, Y_STATE, ZOOM, STACK);
			}
		}
		ofCircle(x, y, 10);
	}
	else if (gestureFinder.blobs.size() == 2)
	{
		float euclidean;
		ofxCvBlob &first = gestureFinder.blobs[0];
		ofxCvBlob &second = gestureFinder.blobs[1];
		int x1 = first.centroid.x;
		int y1 = first.centroid.y;
		int x2 = second.centroid.x;
		int y2 = second.centroid.y;
		ofCircle(x1, y1, 10);
		ofCircle(x2, y2, 10);
		euclidean = ofDist(x1, y1, x2, y2);
//		cout << "Distance (Raw): " << euclidean << endl;
		float zoom = ofMap(euclidean, 0, 800, 0, 1);
//		cout << "Zoom (Mapped: " << zoom << endl;
		ZOOM = zoom;
		sender.send_stack(X_STATE, Y_STATE, ZOOM, STACK);
	}
	else if (isActivated)
	{
		isActivated = false;
		HW_FIRST_LEG = -1;
		HW_SECOND_LEG = -1;
		HW_CURRENT_DIRECTION = -1;
	}
	else
	{
		HW_FIRST_LEG = -1;
		HW_SECOND_LEG = -1;
		HW_CURRENT_DIRECTION = -1;
		HW_FIRST_Y = -1;
		HW_FIRST_X = -1;
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
	gestureImage.draw(700, 20, 400, 300);
	gestureFinder.draw(700, 20, 400, 300);
}

void medical::exit()
{
	kinect.close();
}

void medical::calculateFramerate()
{
	myTimeNow = ofGetElapsedTimef();
	if ((myTimeNow - myTimeThen) > 0.05f || myFrames == 0)
	{
		myFPS = myFrames / (myTimeNow - myTimeThen);
		myTimeThen = myTimeNow;
		myFrames = 0;
		myFramerate = 0.9f * myFramerate + 0.1f * myFPS;
		sprintf(buf, "Framerate: %f", myFramerate);
	}
	++myFrames;
}

void medical::hoverWidget(int x, int y)
{
	if (HW_CURRENT_DIRECTION == -1)
	{
		HW_FIRST_X = x;
		HW_FIRST_Y = y;
		HW_CURRENT_DIRECTION = DIRECTION_WAIT;
	}
	else if (HW_CURRENT_DIRECTION == DIRECTION_WAIT)
	{
		if (x < (HW_FIRST_X - HW_SPACE))
		{
//			cout << "Set direction left" << endl;
			HW_CURRENT_DIRECTION = DIRECTION_LEFT;
			HW_FIRST_X = x;
			HW_FIRST_Y = y;
		}
		else if (x > (HW_FIRST_X + HW_SPACE))
		{
//			cout << "Set direction right" << endl;
			HW_CURRENT_DIRECTION = DIRECTION_RIGHT;
			HW_FIRST_X = x;
			HW_FIRST_Y = y;
		}
		else if (y > (HW_FIRST_Y + HW_SPACE))
		{
//			cout << "Set direction down" << endl;
			HW_CURRENT_DIRECTION = DIRECTION_DOWN;
			HW_FIRST_X = x;
			HW_FIRST_Y = y;
		}
		else if (y < (HW_FIRST_Y - HW_SPACE))
		{
//			cout << "Set direction up" << endl;
			HW_CURRENT_DIRECTION = DIRECTION_UP;
			HW_FIRST_X = x;
			HW_FIRST_Y = y;
		}
	}
	else
	{
//		cout << "Direction is: " << HW_CURRENT_DIRECTION << endl;
		int result = inCorridor(HW_CURRENT_DIRECTION, x, y);
		switch (result)
		{
			case 0:
				HW_CURRENT_DIRECTION = -1;
//				ofSetColor(255, 0, 0);
				break;
			case 1:
//				ofSetColor(0, 255, 0);
				isActivated = true;
			default:
				break;
		}
	}
}

void medical::printDebug(int x, int y)
{
	cout << "x: " << x << ", first x: " << HW_FIRST_X << ", y: " << y << ", first y: " << HW_FIRST_Y << endl;
}

int medical::inCorridor(int DIRECTION, int x, int y)
{
	switch (DIRECTION)
	{
		case DIRECTION_UP:
			if ((x <= (HW_FIRST_X + HW_SPACE)) && (x >= (HW_FIRST_X - HW_SPACE))
				&& (y >= (HW_FIRST_Y - HW_DISTANCE)))
			{
				ofSetColor(255, 255, 0);
				HW_CURRENT_DIRECTION = DIRECTION_UP;
				if (y <= (HW_FIRST_Y - (HW_DISTANCE - HW_SPACE)))
				{
					cout << "WE WENT UP" << endl;
					ofSetColor(0, 255, 255);
					if (HW_FIRST_LEG == -1)
					{
						HW_FIRST_LEG = DIRECTION_UP;
						ofSetColor(255, 255, 0);
						return 0;
					}
					else
					{
						HW_SECOND_LEG = DIRECTION_UP;
						return 1;
					}
				}
			}
			else
				return 0;
			break;
		case DIRECTION_DOWN:
//			if ((x <= (HW_FIRST_X + HW_SPACE)) && (x >= (HW_FIRST_X - HW_SPACE)
			if ((x <= (HW_FIRST_X + HW_SPACE)) && (x >= (HW_FIRST_X - HW_SPACE))
				&& (y <= (HW_FIRST_Y + HW_DISTANCE)))
			{
//				cout << "ENTERING DOWN MODE!" << endl;
				ofSetColor(255, 255, 0);
				HW_CURRENT_DIRECTION = DIRECTION_DOWN;
//				cout << "Y: " << y << " first y: " << HW_FIRST_Y << ", " << (HW_DISTANCE - HW_SPACE) << endl;
				if (y >= (HW_FIRST_Y - (HW_DISTANCE - HW_SPACE)))
				{
					cout << "WE WENT DOWN" << endl;
					ofSetColor(0, 255, 255);
					if (HW_FIRST_LEG == -1)
					{
						HW_FIRST_LEG = DIRECTION_DOWN;
						return 0;
					}
					else
					{
						HW_SECOND_LEG = DIRECTION_DOWN;
						return 1;
					}
				}
			}
			else
			{
				return 0;
			}
			break;
		case DIRECTION_RIGHT:
			if ((y <= (HW_FIRST_Y + HW_SPACE)) && (y >= (HW_FIRST_Y - HW_SPACE))
				&& (x <= (HW_FIRST_X + HW_DISTANCE)))
			{
				ofSetColor(255, 255, 0);
				HW_CURRENT_DIRECTION = DIRECTION_RIGHT;
				if (x >= (HW_FIRST_X + (HW_DISTANCE - HW_SPACE)))
				{
					cout << "WE WENT RIGHT" << endl;
					ofSetColor(0, 255, 255);
					if (HW_FIRST_LEG == -1)
					{
						HW_FIRST_LEG = DIRECTION_RIGHT;
						return 0;
					}
					else
					{
						HW_SECOND_LEG = DIRECTION_RIGHT;
						return 1;
					}
				}
			}
			else
				return 0;
			break;
		case DIRECTION_LEFT:
			if ((y <= (HW_FIRST_Y + HW_SPACE)) && (y >= (HW_FIRST_Y - HW_SPACE))
				&& (x >= (HW_FIRST_X - HW_DISTANCE)))
			{
				ofSetColor(255, 255, 0);
				HW_CURRENT_DIRECTION = DIRECTION_LEFT;
				if (x <= (HW_FIRST_X - (HW_DISTANCE - HW_SPACE)))
				{
					cout << "WE WENT LEFT" << endl;
					ofSetColor(0, 255, 255);
					if (HW_FIRST_LEG == -1)
					{
						HW_FIRST_LEG = DIRECTION_LEFT;
						return 0;
					}
					else
					{
						HW_SECOND_LEG = DIRECTION_LEFT;
						return 1;
					}
				}
			}
			else
				return 0;
			break;
		default:
			return 0;
	}
	return -1;
}

void medical::windowResized(int w, int h)
{
	
}

void medical::keyPressed(int key)
{
	cout << key << " has been pressed.\n";
	switch (key)
	{
		case OF_KEY_UP:
			++angle;
			if (angle > 30) 
				angle = 30;
			kinect.setCameraTiltAngle(angle);
			break;
		case OF_KEY_DOWN:
			--angle;
			if (angle < -30)
				angle = -30;
			kinect.setCameraTiltAngle(angle);
			break;
		default:
			break;
	}
	cout << "Image Display Number is now: " << imageDisplayNumber << "\n";
}
