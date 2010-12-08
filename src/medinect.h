#ifndef _MEDINECT
#define _MEDINECT

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"

class medinect : public ofBaseApp
{
	public:
		void setup();
		void draw();
		void update();
		void exit();
		
		void windowResized(int w, int h);
		void keyPressed(int key);
		
		ofxKinect kinect;
		
		ofxCvContourFinder	contourFinder;
		
		ofxCvGrayscaleImage	grayImage;
		ofxCvGrayscaleImage	grayThresh;
		ofxCvGrayscaleImage	grayThreshFar;
		
		int	nearThreshold;
		int	farThreshold;
		
		ofImage image1;
		ofImage image2;
	
	private:
	
		int imageDisplayNumber;
};

#endif
