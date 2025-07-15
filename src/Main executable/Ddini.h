/***********************************************************************
 *DirectDraw Display Initialization Module (Raylib Compatibility)
 *
 * This module has been migrated from DirectDraw to Raylib.
 * The interface remains the same for compatibility.
 *
 ***********************************************************************/
#ifndef __DDINI_H_
#define __DDINI_H_

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdlib.h>
#include <cstdarg>
#include "resource.h"
#include "lines.h"

// Use the DirectDraw compatibility layer
#include "../graphics/directdraw_compat.h"

// Memory management (kept from original)
#define free _ExFree
#define malloc _ExMalloc
void* _ExMalloc(int Size);
void _ExFree(void* Ptr);

// operator new/delete defined in common.h

#define MAKE_PTC

// External variables
extern HWND                    hwnd;
extern bool                    window_mode;
extern bool                    PalDone;

// Screen buffer helper functions
void SetScreenPtr(void);

// Additional compatibility functions
void ClearScreen(void);
void SERROR();
void SERROR1();
void SERROR2();

// All functions and variables are now provided by the compatibility layer
// and are included via the header above

#endif
