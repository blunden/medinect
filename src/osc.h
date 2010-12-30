#ifndef _OSCSEND
#define _OSCSEND

#include "ofxOsc.h"

class oscsend
{
        public:
                ofxOscSender sender;
		bool osc_initialized;

		void osc_init(std::string, int);
		void send_stack(int, float, float, float, int);
};
#endif
