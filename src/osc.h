#ifndef _OSCSEND
#define _OSCSEND

#include "ofxOsc.h"

class oscsend
{
        public:
                ofxOscSender sender;
		bool osc_initialized;

		int PEN_UP;
		int PEN_DOWN;
		int PEN_MOVE;
		int PEN_UNDO;

		void osc_init(std::string, int);
		void send_stack(int, int, float, int);
		void send_cursor(int, int);
		void send_pen(int, int, int, int, int);
	private:
		int KINECT_ID;
};
#endif
