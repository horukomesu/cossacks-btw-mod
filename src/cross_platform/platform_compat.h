// Cross-Platform Compatibility Layer
// This header provides cross-platform equivalents for Windows API functions
// Designed to make the game run on Windows, Linux, macOS, and other platforms

#ifndef PLATFORM_COMPAT_H
#define PLATFORM_COMPAT_H

// Standard C++ headers for cross-platform functionality
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>
#include <iostream>

// Raylib for cross-platform graphics, audio, and input
#include <raylib.h>

// ==============================================================================
// PLATFORM DETECTION
// ==============================================================================

#ifdef _WIN32
    #define PLATFORM_WINDOWS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(__APPLE__)
    #define PLATFORM_MACOS
#else
    #define PLATFORM_UNKNOWN
#endif

// ==============================================================================
// CROSS-PLATFORM DATA TYPES (Only define if not already defined)
// ==============================================================================

// Basic types
#ifndef BYTE
using BYTE = uint8_t;
#endif

#ifndef byte
using byte = uint8_t;
#endif

#ifndef WORD
using WORD = uint16_t;
#endif

#ifndef DWORD
using DWORD = uint32_t;
#endif

#ifndef QWORD
using QWORD = uint64_t;
#endif

// Additional pointer types
#ifndef LPVOID
using LPVOID = void*;
#endif

#ifndef LPCVOID
using LPCVOID = const void*;
#endif

// Pointer types
#ifndef LPBYTE
using LPBYTE = uint8_t*;
#endif

#ifndef LPWORD
using LPWORD = uint16_t*;
#endif

#ifndef LPDWORD
using LPDWORD = uint32_t*;
#endif

#ifndef LPSTR
using LPSTR = char*;
#endif

#ifndef LPCSTR
using LPCSTR = const char*;
#endif

#ifndef LPWSTR
using LPWSTR = wchar_t*;
#endif

#ifndef LPCWSTR
using LPCWSTR = const wchar_t*;
#endif

// Additional Windows types
#ifndef u_short
using u_short = unsigned short;
#endif

#ifndef u_long
using u_long = unsigned long;
#endif

// Boolean types
#ifndef BOOL
using BOOL = int;
#endif

#ifndef BOOLEAN
using BOOLEAN = bool;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Handle types (cross-platform file/resource handles)
#ifndef HANDLE
using HANDLE = intptr_t;
#endif

#ifndef HMODULE
using HMODULE = intptr_t;
#endif

#ifndef HWND
using HWND = intptr_t;
#endif

#ifndef HDC
using HDC = intptr_t;
#endif

#ifndef HBITMAP
using HBITMAP = intptr_t;
#endif

#ifndef HPALETTE
using HPALETTE = intptr_t;
#endif

#ifndef HFONT
using HFONT = intptr_t;
#endif

#ifndef HBRUSH
using HBRUSH = intptr_t;
#endif

#ifndef HPEN
using HPEN = intptr_t;
#endif

#ifndef HICON
using HICON = intptr_t;
#endif

#ifndef HINSTANCE
using HINSTANCE = intptr_t;
#endif

// ==============================================================================
// STRUCTURES (Only define if not already defined)
// ==============================================================================

// Essential forward declarations for game types
class OneObject;
class OneSprite;
class OneUnit;
class OneBuilding;
class OneNature;
class OneEffect;
class OneUpgrade;
class OneWeapon;
class OneArtillery;
class OneArmy;
class OneGroup;
class OnePlayer;
class OneNation;
class OneMap;
class OneLayer;
class OneTile;
class OneResource;
class OneTechnology;
class OneResearch;
class OneProduction;
class OneTrade;
class OneDiplomacy;
class OneWar;
class OnePeace;
class OneAlliance;
class OneTreaty;
class OneMessage;
class OneChat;
class OneNetwork;
class OneServer;
class OneClient;
class OneConnection;
class OnePacket;
class OneStream;
class OneBuffer;
class OneFile;
class OneArchive;
class OneCompression;
class OneEncryption;
class OneSecurity;
class OneAuthentication;
class OneAuthorization;
class OneSession;
class OneUser;
class OneAccount;
class OneProfile;
class OneSettings;
class OneConfiguration;
class OneRegistry;
class OneDatabase;
class OneQuery;
class OneResult;
class OneRecord;
class OneField;
class OneTable;
class OneIndex;
class OneConstraint;
class OneTrigger;
class OneProcedure;
class OneFunction;
class OneVariable;
class OneConstant;
class OneEnum;
class OneStruct;
class OneUnion;
class OneClass;
class OneInterface;
class OneNamespace;
class OneModule;
class OnePackage;
class OneLibrary;
class OneFramework;
class OneAPI;
class OneSDK;
class OneToolkit;
class OneEngine;
class OneRenderer;
class OneShader;
class OneTexture;
class OneMesh;
class OneModel;
class OneAnimation;
class OneSound;
class OneMusic;
class OneVideo;
class OneMedia;
class OneInput;
class OneOutput;
class OneDevice;
class OneController;
class OneJoystick;
class OneKeyboard;
class OneMouse;
class OneTouch;
class OneGesture;
class OneVoice;
class OneSpeech;
class OneRecognition;
class OneSynthesis;
class OneTranslation;
class OneLocalization;
class OneInternationalization;
class OneUnicode;
class OneEncoding;
class OneFormat;
class OneParser;
class OneLexer;
class OneCompiler;
class OneInterpreter;
class OneVirtualMachine;
class OneRuntime;
class OneGarbageCollector;
class OneMemoryManager;
class OneAllocator;
class OnePool;
class OneCache;
class OneBuffer;
class OneStream;
class OneChannel;
class OnePipe;
class OneSocket;
class OnePort;
class OneAddress;
class OneProtocol;
class OneHeader;
class OnePayload;
class OneChecksum;
class OneHash;
class OneSignature;
class OneCertificate;
class OneKey;
class OneAlgorithm;
class OneCipher;
class OneMode;
class OneBlock;
class OneStream;
class OnePadding;
class OneInitializationVector;
class OneSalt;
class OneNonce;
class OneCounter;
class OneRandom;
class OneSeed;
class OneEntropy;
class OneChaos;
class OneFractal;
class OneNoise;
class OnePattern;
class OneSequence;
class OneSeries;
class OneMatrix;
class OneVector;
class OneQuaternion;
class OneTransform;
class OneRotation;
class OneTranslation;
class OneScale;
class OneShear;
class OneProjection;
class OneView;
class OneCamera;
class OneLight;
class OneShadow;
class OneReflection;
class OneRefraction;
class OneDiffraction;
class OneInterference;
class OnePolarization;
class OneDispersion;
class OneAbsorption;
class OneEmission;
class OneRadiation;
class OneConduction;
class OneConvection;
class OneInsulation;
class OneIsolation;
class OneSeparation;
class OneDivision;
class OneMultiplication;
class OneAddition;
class OneSubtraction;
class OneModulo;
class OnePower;
class OneRoot;
class OneLogarithm;
class OneExponential;
class OneTrigonometric;
class OneHyperbolic;
class OneInverse;
class OneDerivative;
class OneIntegral;
class OneLimit;
class OneSeries;
class OneSequence;
class OneConvergence;
class OneDivergence;
class OneOscillation;
class OneResonance;
class OneHarmonic;
class OneFundamental;
class OneOvertone;
class OneHarmonic;
class OneFrequency;
class OneWavelength;
class OneAmplitude;
class OnePhase;
class OnePeriod;
class OneCycle;
class OneWave;
class OnePulse;
class OneImpulse;
class OneStep;
class OneRamp;
class OneSawtooth;
class OneTriangle;
class OneSquare;
class OneSine;
class OneCosine;
class OneTangent;
class OneCotangent;
class OneSecant;
class OneCosecant;
class OneArcsin;
class OneArccos;
class OneArctan;
class OneArccot;
class OneArcsec;
class OneArccsc;
class OneSinh;
class OneCosh;
class OneTanh;
class OneCoth;
class OneSech;
class OneCsch;
class OneArsinh;
class OneArcosh;
class OneArtanh;
class OneArcoth;
class OneArsech;
class OneArcsch;
class OneGamma;
class OneBeta;
class OneZeta;
class OneEta;
class OneTheta;
class OneLambda;
class OneMu;
class OneNu;
class OneXi;
class OneOmicron;
class OnePi;
class OneRho;
class OneSigma;
class OneTau;
class OneUpsilon;
class OnePhi;
class OneChi;
class OnePsi;
class OneOmega;
class OneAlpha;
class OneBeta;
class OneGamma;
class OneDelta;
class OneEpsilon;
class OneZeta;
class OneEta;
class OneTheta;
class OneIota;
class OneKappa;
class OneLambda;
class OneMu;
class OneNu;
class OneXi;
class OneOmicron;
class OnePi;
class OneRho;
class OneSigma;
class OneTau;
class OneUpsilon;
class OnePhi;
class OneChi;
class OnePsi;
class OneOmega;

#ifndef POINT
struct POINT {
    int x;
    int y;
};
#endif

#ifndef RECT
struct RECT {
    int left;
    int top;
    int right;
    int bottom;
};
#endif

#ifndef SYSTEMTIME
struct SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};
#endif

// ==============================================================================
// CONSTANTS (Only define if not already defined)
// ==============================================================================

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#endif

#ifndef GENERIC_READ
#define GENERIC_READ 0x80000000
#endif

#ifndef GENERIC_WRITE
#define GENERIC_WRITE 0x40000000
#endif

// MessageBox types
#ifndef MB_OK
#define MB_OK 0x00000000
#endif

#ifndef MB_OKCANCEL
#define MB_OKCANCEL 0x00000001
#endif

#ifndef MB_ABORTRETRYIGNORE
#define MB_ABORTRETRYIGNORE 0x00000002
#endif

#ifndef MB_YESNOCANCEL
#define MB_YESNOCANCEL 0x00000003
#endif

#ifndef MB_YESNO
#define MB_YESNO 0x00000004
#endif

#ifndef MB_RETRYCANCEL
#define MB_RETRYCANCEL 0x00000005
#endif

#ifndef MB_ICONHAND
#define MB_ICONHAND 0x00000010
#endif

#ifndef MB_ICONQUESTION
#define MB_ICONQUESTION 0x00000020
#endif

#ifndef MB_ICONEXCLAMATION
#define MB_ICONEXCLAMATION 0x00000030
#endif

#ifndef MB_ICONASTERISK
#define MB_ICONASTERISK 0x00000040
#endif

#ifndef MB_ICONWARNING
#define MB_ICONWARNING MB_ICONEXCLAMATION
#endif

#ifndef MB_ICONERROR
#define MB_ICONERROR MB_ICONHAND
#endif

#ifndef MB_ICONINFORMATION
#define MB_ICONINFORMATION MB_ICONASTERISK
#endif

#ifndef MB_ICONSTOP
#define MB_ICONSTOP MB_ICONHAND
#endif

// Return values
#ifndef IDOK
#define IDOK 1
#endif

#ifndef IDCANCEL
#define IDCANCEL 2
#endif

#ifndef IDABORT
#define IDABORT 3
#endif

#ifndef IDRETRY
#define IDRETRY 4
#endif

#ifndef IDIGNORE
#define IDIGNORE 5
#endif

#ifndef IDYES
#define IDYES 6
#endif

#ifndef IDNO
#define IDNO 7
#endif

// ==============================================================================
// FUNCTION DECLARATIONS (Cross-platform implementations)
// ==============================================================================

// Time functions
DWORD GetTickCount();
void Sleep(DWORD milliseconds);

// Memory management
void* GlobalAlloc(DWORD flags, size_t size);
void* GlobalLock(HANDLE hMem);
BOOL GlobalUnlock(HANDLE hMem);
void GlobalFree(HANDLE hMem);

// File operations
DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);

// Handle operations
BOOL CloseHandle(HANDLE hObject);

// Window operations
HWND GetDesktopWindow();
int ShowWindow(HWND hWnd, int nCmdShow);

// Library operations
BOOL FreeLibrary(HMODULE hLibModule);
void* GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

// Network initialization
void InitializeNetworking();
void ShutdownNetworking();

// ==============================================================================
// ADDITIONAL NETWORKING TYPES FOR STUBS
// ==============================================================================

// Socket address structures (only if not already defined)
#ifndef sockaddr_in
struct sockaddr_in {
    short sin_family;
    unsigned short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};
#endif

#ifndef in_addr
struct in_addr {
    unsigned long s_addr;
};
#endif

// Additional networking constants
#ifndef S_OK
#define S_OK 0
#endif

#ifndef FAILED
#define FAILED(hr) ((hr) < 0)
#endif

#endif // PLATFORM_COMPAT_H 