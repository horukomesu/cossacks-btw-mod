#pragma once

#include <raylib.h>
#include <string>
#include <chrono>

// Raylib Utilities - Cross-platform replacements for Windows API functions
class RaylibUtils
{
public:
    RaylibUtils();
    ~RaylibUtils();
    
    // Initialization
    bool Initialize();
    void Shutdown();
    
    // Timing functions (replace GetTickCount, GetSystemTime, etc.)
    unsigned long GetTickCount();
    unsigned long GetRealTime();
    void GetSystemTime(int* year, int* month, int* day, int* hour, int* minute, int* second, int* day_of_week);
    void Sleep(unsigned long milliseconds);
    
    // File system functions
    bool FileExists(const std::string& filepath);
    bool DirectoryExists(const std::string& dirpath);
    bool CreateDirectory(const std::string& dirpath);
    bool DeleteFile(const std::string& filepath);
    bool CopyFile(const std::string& source, const std::string& destination);
    bool MoveFile(const std::string& source, const std::string& destination);
    long long GetFileSize(const std::string& filepath);
    bool GetFileModifiedTime(const std::string& filepath, int* year, int* month, int* day, int* hour, int* minute, int* second);
    
    // String utilities
    std::string GetExecutablePath();
    std::string GetWorkingDirectory();
    bool SetWorkingDirectory(const std::string& path);
    std::string GetTempDirectory();
    std::string GetUserDocumentsDirectory();
    std::string GetApplicationDataDirectory();
    
    // Math utilities
    float ClampFloat(float value, float min, float max);
    int ClampInt(int value, int min, int max);
    float LerpFloat(float a, float b, float t);
    int RandomInt(int min, int max);
    float RandomFloat(float min, float max);
    
    // Color utilities
    Color HSVtoRGB(float h, float s, float v);
    void RGBtoHSV(Color color, float* h, float* s, float* v);
    Color BlendColors(Color color1, Color color2, float alpha);
    
    // Memory utilities
    void* AllocateMemory(size_t size);
    void FreeMemory(void* ptr);
    void* ReallocateMemory(void* ptr, size_t new_size);
    void ZeroMemory(void* ptr, size_t size);
    void CopyMemory(void* dest, const void* src, size_t size);
    
    // Registry/Settings utilities (cross-platform settings storage)
    bool WriteSettingString(const std::string& key, const std::string& value);
    bool ReadSettingString(const std::string& key, std::string& value);
    bool WriteSettingInt(const std::string& key, int value);
    bool ReadSettingInt(const std::string& key, int& value);
    bool WriteSettingFloat(const std::string& key, float value);
    bool ReadSettingFloat(const std::string& key, float& value);
    bool DeleteSetting(const std::string& key);
    
    // Process utilities
    bool LaunchProcess(const std::string& executable, const std::string& arguments);
    bool IsProcessRunning(const std::string& process_name);
    void ExitApplication(int exit_code);
    
    // Error handling
    std::string GetLastError() const;
    
private:
    std::string last_error;
    bool initialized;
    std::chrono::steady_clock::time_point start_time;
    
    // Helper functions
    void LogError(const std::string& error);
    std::string GetSettingsFilePath();
    void LoadSettings();
    void SaveSettings();
    
    // Settings storage
    std::map<std::string, std::string> settings;
    bool settings_dirty;
};

// Global utils instance
extern RaylibUtils* g_utils;

// C-style interface for compatibility with original code
extern "C" {
    // Initialization
    void InitRaylibUtils();
    void CleanupRaylibUtils();
    
    // Timing
    unsigned long GetRaylibTickCount();
    unsigned long GetRaylibRealTime();
    void GetRaylibSystemTime(int* year, int* month, int* day, int* hour, int* minute, int* second, int* day_of_week);
    void RaylibSleep(unsigned long milliseconds);
    
    // File system
    bool RaylibFileExists(const char* filepath);
    bool RaylibDirectoryExists(const char* dirpath);
    bool RaylibCreateDirectory(const char* dirpath);
    bool RaylibDeleteFile(const char* filepath);
    bool RaylibCopyFile(const char* source, const char* destination);
    bool RaylibMoveFile(const char* source, const char* destination);
    long long RaylibGetFileSize(const char* filepath);
    bool RaylibGetFileModifiedTime(const char* filepath, int* year, int* month, int* day, int* hour, int* minute, int* second);
    
    // String utilities
    const char* RaylibGetExecutablePath();
    const char* RaylibGetWorkingDirectory();
    bool RaylibSetWorkingDirectory(const char* path);
    const char* RaylibGetTempDirectory();
    const char* RaylibGetUserDocumentsDirectory();
    const char* RaylibGetApplicationDataDirectory();
    
    // Math utilities
    float RaylibClampFloat(float value, float min, float max);
    int RaylibClampInt(int value, int min, int max);
    float RaylibLerpFloat(float a, float b, float t);
    int RaylibRandomInt(int min, int max);
    float RaylibRandomFloat(float min, float max);
    
    // Color utilities
    void RaylibHSVtoRGB(float h, float s, float v, unsigned char* r, unsigned char* g, unsigned char* b);
    void RaylibRGBtoHSV(unsigned char r, unsigned char g, unsigned char b, float* h, float* s, float* v);
    void RaylibBlendColors(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1,
                           unsigned char r2, unsigned char g2, unsigned char b2, unsigned char a2,
                           float alpha, unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);
    
    // Memory utilities
    void* RaylibAllocateMemory(size_t size);
    void RaylibFreeMemory(void* ptr);
    void* RaylibReallocateMemory(void* ptr, size_t new_size);
    void RaylibZeroMemory(void* ptr, size_t size);
    void RaylibCopyMemory(void* dest, const void* src, size_t size);
    
    // Settings utilities
    bool RaylibWriteSettingString(const char* key, const char* value);
    bool RaylibReadSettingString(const char* key, char* value, int max_length);
    bool RaylibWriteSettingInt(const char* key, int value);
    bool RaylibReadSettingInt(const char* key, int* value);
    bool RaylibWriteSettingFloat(const char* key, float value);
    bool RaylibReadSettingFloat(const char* key, float* value);
    bool RaylibDeleteSetting(const char* key);
    
    // Process utilities
    bool RaylibLaunchProcess(const char* executable, const char* arguments);
    bool RaylibIsProcessRunning(const char* process_name);
    void RaylibExitApplication(int exit_code);
}

// Utility macros for compatibility
#define RAYLIB_MAX_PATH 4096
#define RAYLIB_INVALID_HANDLE_VALUE (-1)

// Common constants
#define RAYLIB_UTILS_SUCCESS 0
#define RAYLIB_UTILS_ERROR -1

// Path separator
#ifdef _WIN32
    #define RAYLIB_PATH_SEPARATOR "\\"
#else
    #define RAYLIB_PATH_SEPARATOR "/"
#endif 