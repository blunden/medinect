#include "osc.h"

void oscsend::osc_init(string destination_ip, int destination_port)
{
	sender.setup(destination_ip, destination_port);
	osc_initialized = true;
	KINECT_ID = 50;
	PEN_UP = 0;
	PEN_DOWN = 1;
	PEN_MOVE = 2;
	PEN_UNDO = 3;
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
	m.addIntArg(KINECT_ID);
	m.addIntArg(x);
	m.addIntArg(y);
	sender.sendMessage(m);
}

void oscsend::send_pen(int PEN_COMMAND, int x, int y, int color, int stroke)
{
	if (!osc_initialized)
		return;
	
	ofxOscMessage m;
	m.setAddress("/pen");
	m.addIntArg(KINECT_ID);
	m.addIntArg(PEN_COMMAND);
	if (PEN_COMMAND == PEN_UNDO)
	{
		sender.sendMessage(m);
		return;
	}
	m.addIntArg(x);
	m.addIntArg(y);
	if (PEN_COMMAND == PEN_UP)
	{
		sender.sendMessage(m);
		return;
	}
	m.addIntArg(color);
	m.addIntArg(stroke);
	sender.sendMessage(m);
}
