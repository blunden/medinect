#ifndef _MEDICAL
#define _MEDICAL

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"

class medical : public ofBaseApp
{
	public:
		void setup();
		void draw();
		void update();
		void exit();
		
		void windowResized(int w, int h);
		void keyPressed(int key);
		
		ofxKinect kinect;
		
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
	
		int imageDisplayNumber;
		int startX;
		int startY;
		int lastX;
		int lastY;

		bool inYMode;
		bool inActivationMode;
};

#endif
