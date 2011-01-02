#include "osc.h"

void oscsend::osc_init(string destination_ip, int destination_port)
{
	sender.setup(destination_ip, destination_port);
	osc_initialized = true;
	KINECT_ID = 50;
}

void oscsend::send_stack(int x, int y, float z, int i)
{
	//logFile << ofGetElapsedTimeMillis() << "\t" << "/stack\t" << id << "\t" << (int)x << "\t" << (int)y << "\t" << z << "\t" << i << "\t" << endl;
	if (!osc_initialized)
        	return;
	
	ofxOscMessage m;
	m.setAddress("/stack");
        m.addIntArg(KINECT_ID);
        m.addIntArg(x);
        m.addIntArg(y);
        m.addFloatArg(z);
        m.addIntArg(i);
	sender.sendMessage(m);
}

void oscsend::send_cursor(int x, int y)
{
	if (!osc_initialized)
		return;
	
	ofxOscMessage m;
	m.setAddress("/cursor");
	m.addIntArg(x);
	m.addIntArg(y);
	sender.sendMessage(m);
}
