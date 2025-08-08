#include <windows.h>

extern "C" void SendVictoryState( int /*ID*/, byte /*State*/ ) {}
extern "C" void SendPrivateMessage( char* /*Nick*/, char* /*MESSAGE*/ ) {}
extern "C" void GetGameID( char* s ) {
    if (s) {
        s[0] = 0;
    }
}


