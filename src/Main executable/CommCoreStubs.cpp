#ifndef NO_MULTIPLAYER
#define NO_MULTIPLAYER 1
#endif
#include "CommCore.h"

#if defined(NO_MULTIPLAYER)

CCommCore::CCommCore()
{
    lpIdleProc = nullptr;
    lpEnumProc = nullptr;
}

CCommCore::~CCommCore() {}

#endif // NO_MULTIPLAYER

