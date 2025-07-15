// Cross-Platform Compatibility Layer Implementation
// This file implements all the Windows API replacement functions using cross-platform code

#include "platform_compat.h"
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <filesystem>
#include <thread>
#include <chrono>
#include <algorithm>

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <dlfcn.h>
    #include <fcntl.h>
#endif

// ==============================================================================
// INTERNAL STRUCTURES AND GLOBALS
// ==============================================================================

// File handle management
struct FileHandle {
    std::FILE* file;
    std::string filename;
    bool is_valid;
    
    FileHandle() : file(nullptr), is_valid(false) {}
    FileHandle(std::FILE* f, const std::string& name) : file(f), filename(name), is_valid(true) {}
};

// Dynamic library handle management
struct LibraryHandle {
    void* handle;
    std::string filename;
    bool is_valid;
    
    LibraryHandle() : handle(nullptr), is_valid(false) {}
    LibraryHandle(void* h, const std::string& name) : handle(h), filename(name), is_valid(true) {}
};

// Global handle management
static std::map<HANDLE, FileHandle> g_file_handles;
static std::map<HMODULE, LibraryHandle> g_library_handles;
static HANDLE g_next_handle = 1;
static HMODULE g_next_module = 1;

// Timing initialization
static auto g_start_time = std::chrono::steady_clock::now();

// ==============================================================================
// TIMING FUNCTIONS
// ==============================================================================

DWORD GetTickCount() {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_start_time);
    return static_cast<DWORD>(duration.count());
}

void GetSystemTime(SYSTEMTIME* lpSystemTime) {
    if (!lpSystemTime) return;
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    lpSystemTime->wYear = static_cast<WORD>(tm.tm_year + 1900);
    lpSystemTime->wMonth = static_cast<WORD>(tm.tm_mon + 1);
    lpSystemTime->wDayOfWeek = static_cast<WORD>(tm.tm_wday);
    lpSystemTime->wDay = static_cast<WORD>(tm.tm_mday);
    lpSystemTime->wHour = static_cast<WORD>(tm.tm_hour);
    lpSystemTime->wMinute = static_cast<WORD>(tm.tm_min);
    lpSystemTime->wSecond = static_cast<WORD>(tm.tm_sec);
    
    // Get milliseconds
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    lpSystemTime->wMilliseconds = static_cast<WORD>(ms.count());
}

void Sleep(DWORD dwMilliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(dwMilliseconds));
}

// ==============================================================================
// FILE OPERATIONS
// ==============================================================================

HANDLE CreateFile(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
                  void* lpSecurityAttributes, DWORD dwCreationDisposition, 
                  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    
    if (!lpFileName) return INVALID_HANDLE_VALUE;
    
    std::string mode;
    
    // Determine file mode based on access and disposition
    if (dwDesiredAccess & GENERIC_READ && dwDesiredAccess & GENERIC_WRITE) {
        switch (dwCreationDisposition) {
            case CREATE_NEW:
            case CREATE_ALWAYS:
                mode = "w+b";
                break;
            case OPEN_EXISTING:
            case OPEN_ALWAYS:
                mode = "r+b";
                break;
            case TRUNCATE_EXISTING:
                mode = "w+b";
                break;
            default:
                mode = "r+b";
        }
    } else if (dwDesiredAccess & GENERIC_WRITE) {
        mode = "wb";
    } else {
        mode = "rb";
    }
    
    // Handle creation disposition
    if (dwCreationDisposition == CREATE_NEW || dwCreationDisposition == OPEN_EXISTING) {
        if (std::filesystem::exists(lpFileName)) {
            if (dwCreationDisposition == CREATE_NEW) {
                return INVALID_HANDLE_VALUE; // File exists, but we want to create new
            }
        } else {
            if (dwCreationDisposition == OPEN_EXISTING) {
                return INVALID_HANDLE_VALUE; // File doesn't exist, but we want to open existing
            }
        }
    }
    
    std::FILE* file = std::fopen(lpFileName, mode.c_str());
    if (!file) return INVALID_HANDLE_VALUE;
    
    HANDLE handle = g_next_handle++;
    g_file_handles[handle] = FileHandle(file, lpFileName);
    
    return handle;
}

BOOL ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, 
              LPDWORD lpNumberOfBytesRead, void* lpOverlapped) {
    
    auto it = g_file_handles.find(hFile);
    if (it == g_file_handles.end() || !it->second.is_valid) {
        return FALSE;
    }
    
    size_t bytes_read = std::fread(lpBuffer, 1, nNumberOfBytesToRead, it->second.file);
    if (lpNumberOfBytesRead) {
        *lpNumberOfBytesRead = static_cast<DWORD>(bytes_read);
    }
    
    return TRUE;
}

BOOL WriteFile(HANDLE hFile, const void* lpBuffer, DWORD nNumberOfBytesToWrite, 
               LPDWORD lpNumberOfBytesWritten, void* lpOverlapped) {
    
    auto it = g_file_handles.find(hFile);
    if (it == g_file_handles.end() || !it->second.is_valid) {
        return FALSE;
    }
    
    size_t bytes_written = std::fwrite(lpBuffer, 1, nNumberOfBytesToWrite, it->second.file);
    if (lpNumberOfBytesWritten) {
        *lpNumberOfBytesWritten = static_cast<DWORD>(bytes_written);
    }
    
    return (bytes_written == nNumberOfBytesToWrite) ? TRUE : FALSE;
}

BOOL CloseHandle(HANDLE hObject) {
    auto it = g_file_handles.find(hObject);
    if (it != g_file_handles.end() && it->second.is_valid) {
        std::fclose(it->second.file);
        g_file_handles.erase(it);
        return TRUE;
    }
    return FALSE;
}

DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh) {
    auto it = g_file_handles.find(hFile);
    if (it == g_file_handles.end() || !it->second.is_valid) {
        return INVALID_HANDLE_VALUE;
    }
    
    try {
        auto size = std::filesystem::file_size(it->second.filename);
        if (lpFileSizeHigh) {
            *lpFileSizeHigh = static_cast<DWORD>(size >> 32);
        }
        return static_cast<DWORD>(size & 0xFFFFFFFF);
    } catch (const std::exception&) {
        return INVALID_HANDLE_VALUE;
    }
}

BOOL DeleteFile(LPCSTR lpFileName) {
    if (!lpFileName) return FALSE;
    
    try {
        return std::filesystem::remove(lpFileName) ? TRUE : FALSE;
    } catch (const std::exception&) {
        return FALSE;
    }
}

BOOL CopyFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) {
    if (!lpExistingFileName || !lpNewFileName) return FALSE;
    
    try {
        std::filesystem::copy_options options = std::filesystem::copy_options::none;
        if (!bFailIfExists) {
            options = std::filesystem::copy_options::overwrite_existing;
        }
        
        std::filesystem::copy_file(lpExistingFileName, lpNewFileName, options);
        return TRUE;
    } catch (const std::exception&) {
        return FALSE;
    }
}

BOOL MoveFile(LPCSTR lpExistingFileName, LPCSTR lpNewFileName) {
    if (!lpExistingFileName || !lpNewFileName) return FALSE;
    
    try {
        std::filesystem::rename(lpExistingFileName, lpNewFileName);
        return TRUE;
    } catch (const std::exception&) {
        return FALSE;
    }
}

// ==============================================================================
// DIRECTORY OPERATIONS
// ==============================================================================

BOOL CreateDirectory(LPCSTR lpPathName, void* lpSecurityAttributes) {
    if (!lpPathName) return FALSE;
    
    try {
        return std::filesystem::create_directories(lpPathName) ? TRUE : FALSE;
    } catch (const std::exception&) {
        return FALSE;
    }
}

BOOL RemoveDirectory(LPCSTR lpPathName) {
    if (!lpPathName) return FALSE;
    
    try {
        return std::filesystem::remove_all(lpPathName) > 0 ? TRUE : FALSE;
    } catch (const std::exception&) {
        return FALSE;
    }
}

DWORD GetCurrentDirectory(DWORD nBufferLength, LPSTR lpBuffer) {
    if (!lpBuffer) return 0;
    
    try {
        std::string current_dir = std::filesystem::current_path().string();
        if (current_dir.length() + 1 > nBufferLength) {
            return static_cast<DWORD>(current_dir.length() + 1);
        }
        
        std::strcpy(lpBuffer, current_dir.c_str());
        return static_cast<DWORD>(current_dir.length());
    } catch (const std::exception&) {
        return 0;
    }
}

BOOL SetCurrentDirectory(LPCSTR lpPathName) {
    if (!lpPathName) return FALSE;
    
    try {
        std::filesystem::current_path(lpPathName);
        return TRUE;
    } catch (const std::exception&) {
        return FALSE;
    }
}

// ==============================================================================
// MEMORY OPERATIONS
// ==============================================================================

void* GlobalAlloc(DWORD uFlags, size_t dwBytes) {
    return std::malloc(dwBytes);
}

void* GlobalLock(HANDLE hMem) {
    return reinterpret_cast<void*>(hMem);
}

BOOL GlobalUnlock(HANDLE hMem) {
    return TRUE; // No-op for our implementation
}

void GlobalFree(HANDLE hMem) {
    std::free(reinterpret_cast<void*>(hMem));
}

// ==============================================================================
// STRING OPERATIONS
// ==============================================================================

int lstrlen(LPCSTR lpString) {
    return lpString ? static_cast<int>(std::strlen(lpString)) : 0;
}

LPSTR lstrcpy(LPSTR lpString1, LPCSTR lpString2) {
    if (!lpString1 || !lpString2) return lpString1;
    return std::strcpy(lpString1, lpString2);
}

LPSTR lstrcat(LPSTR lpString1, LPCSTR lpString2) {
    if (!lpString1 || !lpString2) return lpString1;
    return std::strcat(lpString1, lpString2);
}

int lstrcmp(LPCSTR lpString1, LPCSTR lpString2) {
    if (!lpString1 || !lpString2) return 0;
    return std::strcmp(lpString1, lpString2);
}

int lstrcmpi(LPCSTR lpString1, LPCSTR lpString2) {
    if (!lpString1 || !lpString2) return 0;
    
    std::string s1 = lpString1;
    std::string s2 = lpString2;
    
    // Convert to lowercase for comparison
    std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
    
    return s1.compare(s2);
}

// ==============================================================================
// DYNAMIC LIBRARY OPERATIONS
// ==============================================================================

HMODULE LoadLibrary(LPCSTR lpLibFileName) {
    if (!lpLibFileName) return 0;
    
    void* handle = nullptr;
    
#ifdef PLATFORM_WINDOWS
    handle = ::LoadLibraryA(lpLibFileName);
#else
    handle = dlopen(lpLibFileName, RTLD_LAZY);
#endif
    
    if (!handle) return 0;
    
    HMODULE module = g_next_module++;
    g_library_handles[module] = LibraryHandle(handle, lpLibFileName);
    
    return module;
}

void* GetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
    if (!lpProcName) return nullptr;
    
    auto it = g_library_handles.find(hModule);
    if (it == g_library_handles.end() || !it->second.is_valid) {
        return nullptr;
    }
    
#ifdef PLATFORM_WINDOWS
    return ::GetProcAddress(static_cast<HMODULE>(it->second.handle), lpProcName);
#else
    return dlsym(it->second.handle, lpProcName);
#endif
}

BOOL FreeLibrary(HMODULE hLibModule) {
    auto it = g_library_handles.find(hLibModule);
    if (it == g_library_handles.end() || !it->second.is_valid) {
        return FALSE;
    }
    
#ifdef PLATFORM_WINDOWS
    ::FreeLibrary(static_cast<HMODULE>(it->second.handle));
#else
    dlclose(it->second.handle);
#endif
    
    g_library_handles.erase(it);
    return TRUE;
}

// ==============================================================================
// USER INTERFACE OPERATIONS
// ==============================================================================

int MessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, DWORD uType) {
    // For now, just print to console
    // In a full implementation, this could use raylib's GUI or a cross-platform dialog library
    
    std::cout << "=== MESSAGE BOX ===" << std::endl;
    if (lpCaption) {
        std::cout << "Title: " << lpCaption << std::endl;
    }
    if (lpText) {
        std::cout << "Message: " << lpText << std::endl;
    }
    std::cout << "===================" << std::endl;
    
    // For now, always return IDOK
    return IDOK;
}

HWND GetDesktopWindow() {
    return reinterpret_cast<HWND>(1); // Dummy handle
}

BOOL ShowWindow(HWND hWnd, int nCmdShow) {
    return TRUE; // No-op for our implementation
}

// ==============================================================================
// PROCESS OPERATIONS
// ==============================================================================

BOOL CreateProcess(LPCSTR lpApplicationName, LPSTR lpCommandLine, 
                   void* lpProcessAttributes, void* lpThreadAttributes,
                   BOOL bInheritHandles, DWORD dwCreationFlags, 
                   void* lpEnvironment, LPCSTR lpCurrentDirectory,
                   void* lpStartupInfo, void* lpProcessInformation) {
    
    // Basic implementation using system()
    if (lpCommandLine) {
        return (std::system(lpCommandLine) == 0) ? TRUE : FALSE;
    }
    
    return FALSE;
}

// ==============================================================================
// NETWORKING OPERATIONS
// ==============================================================================

void InitializeNetworking() {
#ifdef PLATFORM_WINDOWS
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

void ShutdownNetworking() {
#ifdef PLATFORM_WINDOWS
    WSACleanup();
#endif
}

// ==============================================================================
// INITIALIZATION AND CLEANUP
// ==============================================================================

void InitializePlatformCompat() {
    // Initialize timing
    g_start_time = std::chrono::steady_clock::now();
    
    // Initialize networking
    InitializeNetworking();
    
    // Initialize raylib if not already initialized
    if (!IsWindowReady()) {
        // Basic window initialization - this should be customized per application
        InitWindow(800, 600, "Cossacks: Back to War");
        SetTargetFPS(60);
    }
}

void ShutdownPlatformCompat() {
    // Close all open file handles
    for (auto& pair : g_file_handles) {
        if (pair.second.is_valid && pair.second.file) {
            std::fclose(pair.second.file);
        }
    }
    g_file_handles.clear();
    
    // Close all open library handles
    for (auto& pair : g_library_handles) {
        if (pair.second.is_valid && pair.second.handle) {
#ifdef PLATFORM_WINDOWS
            ::FreeLibrary(static_cast<HMODULE>(pair.second.handle));
#else
            dlclose(pair.second.handle);
#endif
        }
    }
    g_library_handles.clear();
    
    // Shutdown networking
    ShutdownNetworking();
    
    // Close raylib window
    if (IsWindowReady()) {
        CloseWindow();
    }
} 