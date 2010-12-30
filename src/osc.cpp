#include "osc.h"

void oscsend::osc_init(string destination_ip, int destination_port)
{
    sender.setup(destination_ip, destination_port);
    osc_initialized = true;
}

void oscsend::send_stack(int id, float x, float y, float z, int i)
{
    //logFile << ofGetElapsedTimeMillis() << "\t" << "/stack\t" << id << "\t" << (int)x << "\t" << (int)y << "\t" << z << "\t" << i << "\t" << endl;

    if (!osc_initialized)
        return;

    ofxOscMessage m;
        m.setAddress( "/stack" );
        m.addIntArg( id );
        m.addIntArg( (int)x );
        m.addIntArg( (int)y );
        m.addFloatArg( z );
        m.addIntArg( i );
    sender.sendMessage( m );
}
