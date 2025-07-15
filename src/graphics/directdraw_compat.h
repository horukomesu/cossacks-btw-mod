// DirectDraw Compatibility Layer for Raylib Migration
// This header provides the same interface as the original DirectDraw system
// but redirects all calls to the raylib graphics system

#ifndef DIRECTDRAW_COMPAT_H
#define DIRECTDRAW_COMPAT_H

#include "../cross_platform/platform_compat.h"
#include <stdlib.h>
#include <cstdarg>
#include "raylib_graphics.h"

// DirectDraw compatibility types
struct IDirectDraw;
struct IDirectDrawSurface;
struct IDirectDrawPalette;
typedef IDirectDraw* LPDIRECTDRAW;
typedef IDirectDrawSurface* LPDIRECTDRAWSURFACE;
typedef IDirectDrawPalette* LPDIRECTDRAWPALETTE;

// DirectDraw compatibility structures and globals
extern LPDIRECTDRAW            lpDD;           // DirectDraw object (null, for compatibility)
extern LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface (null, for compatibility)
extern LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface (null, for compatibility)
extern LPDIRECTDRAWPALETTE     lpDDPal;        // DirectDraw palette (null, for compatibility)
extern bool                    bActive;        // is application active?
extern bool                    CurrentSurface; // for compatibility
extern bool                    DDError;        // =FALSE if graphics work normally 
extern DDSURFACEDESC           ddsd;          // surface descriptor
extern PALETTEENTRY            GPal[256];     // Global palette

// Screen variables - these will be managed by raylib
extern int RealLx;
extern int RealLy;
extern int SCRSizeX;
extern int SCRSizeY;
extern int RSCRSizeX;
extern int RSCRSizeY;
extern int COPYSizeX;
extern int Pitch;

// Screen buffer pointers - these will point to raylib managed memory
extern void* ScreenPtr;
extern void* RealScreenPtr;

// Mode enumeration
extern int ModeLX[32];
extern int ModeLY[32];
extern int NModes;

// DirectDraw wrapper functions - these redirect to raylib
bool CreateDDObjects(HWND hwnd);
void FreeDDObjects(void);
void FlipPages(void);
void LockSurface(void);
void UnlockSurface(void);
void ClearRGB(void);
void ClearScreen(void);

// Palette functions
void LoadPalette(LPCSTR lpFileName);
void SlowLoadPalette(LPCSTR lpFileName);
void SlowUnLoadPalette(LPCSTR lpFileName);
byte GetPaletteColor(int r, int g, int b);
void GetPalColor(byte idx, byte* r, byte* g, byte* b);

// Mode enumeration
bool EnumModesOnly(void);
HRESULT DirectDrawCreate_wrapper(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

// RGB specific functions
BOOL CreateRGBDDObjects(HWND hwnd);
BOOL CreateRGB640DDObjects(HWND hwnd);

// Initialization helpers
bool InitScreen();
bool InitRGBScreen();
bool InitRGB640Screen();

#endif // DIRECTDRAW_COMPAT_H 