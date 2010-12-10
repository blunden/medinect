#include "openFrameworks/ofMain.h"
#include "medical.h"
#include "ofAppGlutWindow.h"

int main()
{
	ofAppGlutWindow window;	
	ofSetupOpenGL(&window, 1200, 700, OF_WINDOW);

	ofRunApp(new medical());
}
