#include "ofMain.h"
#include "medinect.h"
#include "ofAppGlutWindow.h"

int main()
{
	ofAppGlutWindow window;	
	ofSetupOpenGL(&window, 1200, 700, OF_WINDOW);

	ofRunApp(new medinect());
}
