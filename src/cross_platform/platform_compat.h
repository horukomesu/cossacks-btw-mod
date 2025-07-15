// Cross-Platform Compatibility Layer
// This header completely replaces Windows API dependencies with cross-platform equivalents
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
// CROSS-PLATFORM DATA TYPES (Replace Windows types)
// ==============================================================================

// Basic types
using BYTE = uint8_t;
using WORD = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

// Pointer types
using LPBYTE = uint8_t*;
using LPWORD = uint16_t*;
using LPDWORD = uint32_t*;
using LPSTR = char*;
using LPCSTR = const char*;
using LPWSTR = wchar_t*;
using LPCWSTR = const wchar_t*;

// Boolean types
using BOOL = int;
using BOOLEAN = bool;
#define TRUE 1
#define FALSE 0

// Handle types (cross-platform file/resource handles)
using HANDLE = intptr_t;
using HMODULE = intptr_t;
using HWND = intptr_t;
using HDC = intptr_t;
using HBITMAP = intptr_t;
using HPALETTE = intptr_t;
using HFONT = intptr_t;
using HBRUSH = intptr_t;
using HPEN = intptr_t;
using HICON = intptr_t;
using HCURSOR = intptr_t;

// Constants
#define INVALID_HANDLE_VALUE (-1)
#define MAX_PATH 260
#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000
#define FILE_SHARE_READ 0x00000001
#define FILE_SHARE_WRITE 0x00000002
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5

// Structure types
struct POINT {
    int x;
    int y;
};

struct RECT {
    int left;
    int top;
    int right;
    int bottom;
};

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

// ==============================================================================
// CROSS-PLATFORM FUNCTION REPLACEMENTS
// ==============================================================================

// Timing functions
DWORD GetTickCount();
void GetSystemTime(SYSTEMTIME* lpSystemTime);
void Sleep(DWORD dwMilliseconds);

// File operations
HANDLE CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
                  void* lpSecurityAttributes, DWORD dwCreationDisposition, 
                  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, 
              LPDWORD lpNumberOfBytesRead, void* lpOverlapped);
BOOL WriteFile(HANDLE hFile, const void* lpBuffer, DWORD nNumberOfBytesToWrite, 
               LPDWORD lpNumberOfBytesWritten, void* lpOverlapped);
BOOL CloseHandle(HANDLE hObject);
DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
BOOL DeleteFile(LPCSTR lpFileName);
BOOL CopyFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists);
BOOL MoveFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName);

// Directory operations
BOOL CreateDirectory(LPCSTR lpPathName, void* lpSecurityAttributes);
BOOL RemoveDirectory(LPCSTR lpPathName);
DWORD GetCurrentDirectory(DWORD nBufferLength, LPSTR lpBuffer);
BOOL SetCurrentDirectory(LPCSTR lpPathName);

// Memory operations
void* GlobalAlloc(DWORD uFlags, size_t dwBytes);
void* GlobalLock(HANDLE hMem);
BOOL GlobalUnlock(HANDLE hMem);
void GlobalFree(HANDLE hMem);

// String operations
int lstrlen(LPCSTR lpString);
LPSTR lstrcpy(LPSTR lpString1, LPCSTR lpString2);
LPSTR lstrcat(LPSTR lpString1, LPCSTR lpString2);
int lstrcmp(LPCSTR lpString1, LPCSTR lpString2);
int lstrcmpi(LPCSTR lpString1, LPCSTR lpString2);

// Dynamic library operations
HMODULE LoadLibrary(LPCSTR lpLibFileName);
void* GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
BOOL FreeLibrary(HMODULE hLibModule);

// User interface operations
int MessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, DWORD uType);
HWND GetDesktopWindow();
BOOL ShowWindow(HWND hWnd, int nCmdShow);

// MessageBox types
#define MB_OK 0x00000000
#define MB_OKCANCEL 0x00000001
#define MB_ABORTRETRYIGNORE 0x00000002
#define MB_YESNOCANCEL 0x00000003
#define MB_YESNO 0x00000004
#define MB_RETRYCANCEL 0x00000005
#define MB_ICONHAND 0x00000010
#define MB_ICONQUESTION 0x00000020
#define MB_ICONEXCLAMATION 0x00000030
#define MB_ICONASTERISK 0x00000040
#define MB_ICONWARNING MB_ICONEXCLAMATION
#define MB_ICONERROR MB_ICONHAND
#define MB_ICONINFORMATION MB_ICONASTERISK
#define MB_ICONSTOP MB_ICONHAND

// Return values
#define IDOK 1
#define IDCANCEL 2
#define IDABORT 3
#define IDRETRY 4
#define IDIGNORE 5
#define IDYES 6
#define IDNO 7

// Process operations
BOOL CreateProcess(LPCSTR lpApplicationName, LPSTR lpCommandLine, 
                   void* lpProcessAttributes, void* lpThreadAttributes,
                   BOOL bInheritHandles, DWORD dwCreationFlags, 
                   void* lpEnvironment, LPCSTR lpCurrentDirectory,
                   void* lpStartupInfo, void* lpProcessInformation);

// Math operations (replacements for Windows-specific functions)
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

// ==============================================================================
// CROSS-PLATFORM INITIALIZATION AND CLEANUP
// ==============================================================================

// Initialize the cross-platform compatibility layer
void InitializePlatformCompat();

// Cleanup the cross-platform compatibility layer
void ShutdownPlatformCompat();

// ==============================================================================
// BACKWARDS COMPATIBILITY MACROS
// ==============================================================================

// Remove Windows-specific macros that may cause conflicts
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(__APPLE__)
    #define PLATFORM_MACOS
#elif defined(__unix__)
    #define PLATFORM_UNIX
#else
    #define PLATFORM_UNKNOWN
#endif

// Compiler-specific attributes
#if defined(_MSC_VER)
    #define FORCEINLINE __forceinline
    #define DECLSPEC_DLLIMPORT __declspec(dllimport)
    #define DECLSPEC_DLLEXPORT __declspec(dllexport)
#elif defined(__GNUC__)
    #define FORCEINLINE __attribute__((always_inline)) inline
    #define DECLSPEC_DLLIMPORT
    #define DECLSPEC_DLLEXPORT __attribute__((visibility("default")))
#else
    #define FORCEINLINE inline
    #define DECLSPEC_DLLIMPORT
    #define DECLSPEC_DLLEXPORT
#endif

// Replace Windows-specific function calling conventions
#define PASCAL
#define CALLBACK
#define WINAPI
#define APIENTRY

// ==============================================================================
// NETWORKING COMPATIBILITY
// ==============================================================================

// Cross-platform socket types
#ifdef PLATFORM_WINDOWS
    #include <winsock2.h>
    #include <ws2tcpip.h>
    using SOCKET = ::SOCKET;
    #define INVALID_SOCKET INVALID_SOCKET
    #define SOCKET_ERROR SOCKET_ERROR
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    using SOCKET = int;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
    #define closesocket close
#endif

// Network initialization
void InitializeNetworking();
void ShutdownNetworking();

#endif // PLATFORM_COMPAT_H 