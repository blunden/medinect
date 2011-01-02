#ifndef _MEDICAL
#define _MEDICAL

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"
#include "osc.h"
#include "ofxArgs.h"

class medical : public ofBaseApp
{
	private:
		ofxArgs* args;
		string dest_ip;
		int dest_port;
	public:
		medical(ofxArgs* args);
		void setup();
		void draw();
		void update();
		void exit();
		
		void windowResized(int w, int h);
		void keyPressed(int key);
		
		ofxKinect kinect;
		oscsend sender;
		
		ofxCvContourFinder	gestureFinder;
		
		ofxCvGrayscaleImage	gestureImage;
		ofxCvGrayscaleImage	gestureThresh;
		ofxCvGrayscaleImage	gestureThreshFar;

		ofxCvContourFinder	activationFinder;

		ofxCvGrayscaleImage	activationImage;
		ofxCvGrayscaleImage	activationThresh;
		ofxCvGrayscaleImage	activationThreshFar;
		
		int	nearGestureThreshold;
		int	farGestureThreshold;

		int	nearActivationThreshold;
		int	farActivationThreshold;
		
		ofImage image1;
		ofImage image2;
		
		ofTrueTypeFont verdana;
	
	private:

		void calculateFramerate();
	
		int imageDisplayNumber;
		int startX;
		int startY;
		int lastX;
		int lastY;

		int angle;

		bool inYMode;
		bool inActivationMode;
		bool activated;
		
		char buf[255];
		float myTimeNow, myTimeThen, myFramerate;
		float myFPS;
		float myFrames;
};

#endif
