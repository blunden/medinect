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

		int	nearGestureThreshold;
		int	farGestureThreshold;

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
		
		//Hover Widget stuffs
		int HW_FIRST_X;
		int HW_FIRST_Y;
		int HW_SPACE;
		int HW_DISTANCE;
		int HW_FIRST_LEG;
		int HW_SECOND_LEG;
		int HW_CURRENT_DIRECTION;
		
/*		int DIRECTION_UP;
		int DIRECTION_DOWN;
		int DIRECTION_LEFT;
		int DIRECTION_RIGHT;
		int DIRECTION_WAIT;*/
		
		bool isActivated;
		float ZOOM;
		int STACK;
		int X_STATE;
		int Y_STATE;

		void hoverWidget(int, int);
		int inCorridor(int, int, int);
		void printDebug(int, int);
};

#endif
