#include "ofMain.h"
#include "medical.h"
#include "ofAppGlutWindow.h"
#include "ofxArgs.h"

int main(int argc, char* argv[])
{
	ofxArgs* args = new ofxArgs(argc, argv);
	ofAppGlutWindow window;	
	ofSetupOpenGL(&window, 1200, 700, OF_WINDOW);

	ofRunApp(new medical(args));
	delete args;
}
