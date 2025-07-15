// DirectDraw Compatibility Layer for Raylib Migration
// This implementation provides the same interface as the original DirectDraw system
// but redirects all calls to the raylib graphics system

#include "directdraw_compat.h"
#include "raylib_graphics.h"
#include "../utils/raylib_utils.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

// DirectDraw compatibility globals (these will be null/empty for compatibility)
LPDIRECTDRAW            lpDD = nullptr;
LPDIRECTDRAWSURFACE     lpDDSPrimary = nullptr;
LPDIRECTDRAWSURFACE     lpDDSBack = nullptr;
LPDIRECTDRAWPALETTE     lpDDPal = nullptr;
bool                    bActive = true;
bool                    CurrentSurface = false;
bool                    DDError = false;
DDSURFACEDESC           ddsd = {0};
PALETTEENTRY            GPal[256];

// Screen variables - managed by raylib
int RealLx = 1024;
int RealLy = 768;
int SCRSizeX = 1024;
int SCRSizeY = 768;
int RSCRSizeX = 1024;
int RSCRSizeY = 768;
int COPYSizeX = 1024;
int Pitch = 1024;

// Screen buffer pointers - point to raylib managed memory
void* ScreenPtr = nullptr;
void* RealScreenPtr = nullptr;

// Mode enumeration
int ModeLX[32] = {0};
int ModeLY[32] = {0};
int NModes = 0;

// External window handle
extern HWND hwnd;
extern bool window_mode;

// Initialize supported display modes
void InitializeModes() {
    // Common resolutions that the game supports
    const int supportedModes[][2] = {
        {1024, 768},
        {1152, 864},
        {1280, 720},
        {1280, 1024},
        {1366, 768},
        {1600, 900},
        {1920, 1080}
    };
    
    NModes = 0;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Only include modes that fit the current display
    for (int i = 0; i < 7; i++) {
        if (supportedModes[i][0] <= screenWidth && supportedModes[i][1] <= screenHeight) {
            ModeLX[NModes] = supportedModes[i][0];
            ModeLY[NModes] = supportedModes[i][1];
            NModes++;
        }
    }
}

// DirectDraw wrapper functions - these redirect to raylib
bool CreateDDObjects(HWND hwnd) {
    // Initialize raylib graphics system
    if (!RaylibGraphics::IsInitialized()) {
        RaylibGraphics::Initialize(RealLx, RealLy, "Cossacks", !window_mode);
    }
    
    // Set up screen variables
    SCRSizeX = RealLx;
    SCRSizeY = RealLy;
    RSCRSizeX = RealLx;
    RSCRSizeY = RealLy;
    COPYSizeX = RealLx;
    Pitch = RealLx;
    
    // Initialize palette with default values
    for (int i = 0; i < 256; i++) {
        GPal[i].peRed = i;
        GPal[i].peGreen = i;
        GPal[i].peBlue = i;
        GPal[i].peFlags = 0;
    }
    
    DDError = false;
    bActive = true;
    
    return true;
}

void FreeDDObjects(void) {
    // Cleanup will be handled by raylib system
    // This is kept for compatibility
}

void FlipPages(void) {
    // Redirect to raylib graphics system
    RaylibGraphics::FlipPages();
}

void LockSurface(void) {
    // Redirect to raylib graphics system
    RaylibGraphics::LockSurface();
    
    // Set up screen pointers for compatibility
    ScreenPtr = RaylibGraphics::GetScreenPtr();
    RealScreenPtr = ScreenPtr;
    
    // Set up surface descriptor for compatibility
    ddsd.lpSurface = ScreenPtr;
    ddsd.lPitch = Pitch;
    ddsd.dwHeight = RSCRSizeY;
}

void UnlockSurface(void) {
    // Redirect to raylib graphics system
    RaylibGraphics::UnlockSurface();
}

void ClearRGB(void) {
    // Redirect to raylib graphics system
    RaylibGraphics::ClearScreen();
}

void ClearScreen(void) {
    // Redirect to raylib graphics system
    RaylibGraphics::ClearScreen();
}

// Palette functions
void LoadPalette(LPCSTR lpFileName) {
    // Load palette using raylib system
    RaylibGraphics::LoadPalette(lpFileName, GPal);
}

void SlowLoadPalette(LPCSTR lpFileName) {
    // For now, just load the palette normally
    // TODO: Implement fade-in effect if needed
    LoadPalette(lpFileName);
}

void SlowUnLoadPalette(LPCSTR lpFileName) {
    // For now, just a no-op
    // TODO: Implement fade-out effect if needed
}

byte GetPaletteColor(int r, int g, int b) {
    int dmax = 10000;
    int bestc = 0;
    for (int i = 0; i < 256; i++) {
        int d = abs(r - GPal[i].peRed) + abs(g - GPal[i].peGreen) + abs(b - GPal[i].peBlue);
        if (d < dmax) {
            dmax = d;
            bestc = i;
        }
    }
    return bestc;
}

void GetPalColor(byte idx, byte* r, byte* g, byte* b) {
    *r = GPal[idx].peRed;
    *g = GPal[idx].peGreen;
    *b = GPal[idx].peBlue;
}

// Mode enumeration
bool EnumModesOnly(void) {
    InitializeModes();
    return true;
}

HRESULT DirectDrawCreate_wrapper(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter) {
    // Return success for compatibility
    return 0; // DD_OK equivalent
}

// RGB specific functions
BOOL CreateRGBDDObjects(HWND hwnd) {
    return CreateDDObjects(hwnd);
}

BOOL CreateRGB640DDObjects(HWND hwnd) {
    return CreateDDObjects(hwnd);
}

// Initialization helpers
bool InitScreen() {
    return CreateDDObjects(hwnd);
}

bool InitRGBScreen() {
    return CreateRGBDDObjects(hwnd);
}

bool InitRGB640Screen() {
    return CreateRGB640DDObjects(hwnd);
} 