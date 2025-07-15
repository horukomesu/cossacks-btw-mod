#include "raylib_utils.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <ctime>
#include <thread>
#include <map>
#include <random>

// Global utils instance
RaylibUtils* g_utils = nullptr;

RaylibUtils::RaylibUtils()
    : initialized(false)
    , settings_dirty(false)
    , start_time(std::chrono::steady_clock::now())
{
}

RaylibUtils::~RaylibUtils()
{
    Shutdown();
}

bool RaylibUtils::Initialize()
{
    try
    {
        LoadSettings();
        initialized = true;
        last_error = "";
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Exception during utils initialization: " + std::string(e.what()));
        return false;
    }
}

void RaylibUtils::Shutdown()
{
    if (!initialized)
        return;
    
    if (settings_dirty)
    {
        SaveSettings();
    }
    
    settings.clear();
    initialized = false;
}

unsigned long RaylibUtils::GetTickCount()
{
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
    return static_cast<unsigned long>(duration.count());
}

unsigned long RaylibUtils::GetRealTime()
{
    return GetTickCount(); // Same as GetTickCount for simplicity
}

void RaylibUtils::GetSystemTime(int* year, int* month, int* day, int* hour, int* minute, int* second, int* day_of_week)
{
    auto now = std::chrono::system_clock::now();
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time_t);
    
    *year = tm->tm_year + 1900;
    *month = tm->tm_mon + 1;
    *day = tm->tm_mday;
    *hour = tm->tm_hour;
    *minute = tm->tm_min;
    *second = tm->tm_sec;
    *day_of_week = tm->tm_wday;
}

void RaylibUtils::Sleep(unsigned long milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

bool RaylibUtils::FileExists(const std::string& filepath)
{
    return std::filesystem::exists(filepath);
}

bool RaylibUtils::DirectoryExists(const std::string& dirpath)
{
    return std::filesystem::exists(dirpath) && std::filesystem::is_directory(dirpath);
}

bool RaylibUtils::CreateDirectory(const std::string& dirpath)
{
    return std::filesystem::create_directories(dirpath);
}

bool RaylibUtils::DeleteFile(const std::string& filepath)
{
    return std::filesystem::remove(filepath);
}

bool RaylibUtils::CopyFile(const std::string& source, const std::string& destination)
{
    try
    {
        std::filesystem::copy_file(source, destination);
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Failed to copy file: " + std::string(e.what()));
        return false;
    }
}

bool RaylibUtils::MoveFile(const std::string& source, const std::string& destination)
{
    try
    {
        std::filesystem::rename(source, destination);
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Failed to move file: " + std::string(e.what()));
        return false;
    }
}

long long RaylibUtils::GetFileSize(const std::string& filepath)
{
    try
    {
        return std::filesystem::file_size(filepath);
    }
    catch (const std::exception& e)
    {
        LogError("Failed to get file size: " + std::string(e.what()));
        return -1;
    }
}

bool RaylibUtils::GetFileModifiedTime(const std::string& filepath, int* year, int* month, int* day, int* hour, int* minute, int* second)
{
    try
    {
        auto ftime = std::filesystem::last_write_time(filepath);
        auto cftime = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now() + (ftime - std::filesystem::file_time_type::clock::now()));
        std::tm* tm = std::localtime(&cftime);
        
        *year = tm->tm_year + 1900;
        *month = tm->tm_mon + 1;
        *day = tm->tm_mday;
        *hour = tm->tm_hour;
        *minute = tm->tm_min;
        *second = tm->tm_sec;
        
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Failed to get file modified time: " + std::string(e.what()));
        return false;
    }
}

std::string RaylibUtils::GetExecutablePath()
{
    return std::filesystem::current_path().string();
}

std::string RaylibUtils::GetWorkingDirectory()
{
    return std::filesystem::current_path().string();
}

bool RaylibUtils::SetWorkingDirectory(const std::string& path)
{
    try
    {
        std::filesystem::current_path(path);
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Failed to set working directory: " + std::string(e.what()));
        return false;
    }
}

std::string RaylibUtils::GetTempDirectory()
{
    return std::filesystem::temp_directory_path().string();
}

std::string RaylibUtils::GetUserDocumentsDirectory()
{
    // This is a simplified implementation
    // In a real implementation, you'd use platform-specific APIs
#ifdef _WIN32
    return "C:\\Users\\Public\\Documents";
#else
    return "/home/user/Documents";
#endif
}

std::string RaylibUtils::GetApplicationDataDirectory()
{
    // This is a simplified implementation
    // In a real implementation, you'd use platform-specific APIs
#ifdef _WIN32
    return "C:\\Users\\Public\\AppData\\Roaming\\Cossacks";
#else
    return "/home/user/.config/cossacks";
#endif
}

float RaylibUtils::ClampFloat(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int RaylibUtils::ClampInt(int value, int min, int max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float RaylibUtils::LerpFloat(float a, float b, float t)
{
    return a + t * (b - a);
}

int RaylibUtils::RandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

float RaylibUtils::RandomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return static_cast<float>(dis(gen));
}

Color RaylibUtils::HSVtoRGB(float h, float s, float v)
{
    return ColorFromHSV(h, s, v);
}

void RaylibUtils::RGBtoHSV(Color color, float* h, float* s, float* v)
{
    Vector3 hsv = ColorToHSV(color);
    *h = hsv.x;
    *s = hsv.y;
    *v = hsv.z;
}

Color RaylibUtils::BlendColors(Color color1, Color color2, float alpha)
{
    return {
        (unsigned char)(color1.r * (1.0f - alpha) + color2.r * alpha),
        (unsigned char)(color1.g * (1.0f - alpha) + color2.g * alpha),
        (unsigned char)(color1.b * (1.0f - alpha) + color2.b * alpha),
        (unsigned char)(color1.a * (1.0f - alpha) + color2.a * alpha)
    };
}

void* RaylibUtils::AllocateMemory(size_t size)
{
    return malloc(size);
}

void RaylibUtils::FreeMemory(void* ptr)
{
    free(ptr);
}

void* RaylibUtils::ReallocateMemory(void* ptr, size_t new_size)
{
    return realloc(ptr, new_size);
}

void RaylibUtils::ZeroMemory(void* ptr, size_t size)
{
    memset(ptr, 0, size);
}

void RaylibUtils::CopyMemory(void* dest, const void* src, size_t size)
{
    memcpy(dest, src, size);
}

bool RaylibUtils::WriteSettingString(const std::string& key, const std::string& value)
{
    settings[key] = value;
    settings_dirty = true;
    return true;
}

bool RaylibUtils::ReadSettingString(const std::string& key, std::string& value)
{
    auto it = settings.find(key);
    if (it != settings.end())
    {
        value = it->second;
        return true;
    }
    return false;
}

bool RaylibUtils::WriteSettingInt(const std::string& key, int value)
{
    return WriteSettingString(key, std::to_string(value));
}

bool RaylibUtils::ReadSettingInt(const std::string& key, int& value)
{
    std::string str_value;
    if (ReadSettingString(key, str_value))
    {
        try
        {
            value = std::stoi(str_value);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
    return false;
}

bool RaylibUtils::WriteSettingFloat(const std::string& key, float value)
{
    return WriteSettingString(key, std::to_string(value));
}

bool RaylibUtils::ReadSettingFloat(const std::string& key, float& value)
{
    std::string str_value;
    if (ReadSettingString(key, str_value))
    {
        try
        {
            value = std::stof(str_value);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
    return false;
}

bool RaylibUtils::DeleteSetting(const std::string& key)
{
    auto it = settings.find(key);
    if (it != settings.end())
    {
        settings.erase(it);
        settings_dirty = true;
        return true;
    }
    return false;
}

bool RaylibUtils::LaunchProcess(const std::string& executable, const std::string& arguments)
{
    // This is a simplified implementation
    std::string command = executable + " " + arguments;
    int result = system(command.c_str());
    return result == 0;
}

bool RaylibUtils::IsProcessRunning(const std::string& process_name)
{
    // This is a simplified implementation
    // In a real implementation, you'd use platform-specific APIs
    LogError("IsProcessRunning not fully implemented");
    return false;
}

void RaylibUtils::ExitApplication(int exit_code)
{
    exit(exit_code);
}

std::string RaylibUtils::GetLastError() const
{
    return last_error;
}

void RaylibUtils::LogError(const std::string& error)
{
    last_error = error;
    std::cerr << "RaylibUtils Error: " << error << std::endl;
}

std::string RaylibUtils::GetSettingsFilePath()
{
    return GetApplicationDataDirectory() + RAYLIB_PATH_SEPARATOR + "settings.cfg";
}

void RaylibUtils::LoadSettings()
{
    std::string filepath = GetSettingsFilePath();
    std::ifstream file(filepath);
    
    if (!file.is_open())
        return;
    
    std::string line;
    while (std::getline(file, line))
    {
        size_t pos = line.find('=');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            settings[key] = value;
        }
    }
    
    file.close();
}

void RaylibUtils::SaveSettings()
{
    std::string filepath = GetSettingsFilePath();
    std::string dir = GetApplicationDataDirectory();
    
    // Create directory if it doesn't exist
    CreateDirectory(dir);
    
    std::ofstream file(filepath);
    if (!file.is_open())
        return;
    
    for (const auto& setting : settings)
    {
        file << setting.first << "=" << setting.second << std::endl;
    }
    
    file.close();
    settings_dirty = false;
}

// C-style interface implementations
extern "C" {

void InitRaylibUtils()
{
    if (!g_utils)
    {
        g_utils = new RaylibUtils();
        g_utils->Initialize();
    }
}

void CleanupRaylibUtils()
{
    if (g_utils)
    {
        delete g_utils;
        g_utils = nullptr;
    }
}

unsigned long GetRaylibTickCount()
{
    return g_utils ? g_utils->GetTickCount() : 0;
}

unsigned long GetRaylibRealTime()
{
    return g_utils ? g_utils->GetRealTime() : 0;
}

void GetRaylibSystemTime(int* year, int* month, int* day, int* hour, int* minute, int* second, int* day_of_week)
{
    if (g_utils)
    {
        g_utils->GetSystemTime(year, month, day, hour, minute, second, day_of_week);
    }
    else
    {
        *year = *month = *day = *hour = *minute = *second = *day_of_week = 0;
    }
}

void RaylibSleep(unsigned long milliseconds)
{
    if (g_utils)
    {
        g_utils->Sleep(milliseconds);
    }
}

bool RaylibFileExists(const char* filepath)
{
    return g_utils ? g_utils->FileExists(filepath) : false;
}

bool RaylibDirectoryExists(const char* dirpath)
{
    return g_utils ? g_utils->DirectoryExists(dirpath) : false;
}

bool RaylibCreateDirectory(const char* dirpath)
{
    return g_utils ? g_utils->CreateDirectory(dirpath) : false;
}

bool RaylibDeleteFile(const char* filepath)
{
    return g_utils ? g_utils->DeleteFile(filepath) : false;
}

bool RaylibCopyFile(const char* source, const char* destination)
{
    return g_utils ? g_utils->CopyFile(source, destination) : false;
}

bool RaylibMoveFile(const char* source, const char* destination)
{
    return g_utils ? g_utils->MoveFile(source, destination) : false;
}

long long RaylibGetFileSize(const char* filepath)
{
    return g_utils ? g_utils->GetFileSize(filepath) : -1;
}

bool RaylibGetFileModifiedTime(const char* filepath, int* year, int* month, int* day, int* hour, int* minute, int* second)
{
    return g_utils ? g_utils->GetFileModifiedTime(filepath, year, month, day, hour, minute, second) : false;
}

// Additional C-style implementations would go here...
// For brevity, I'm including just the essential ones

} // extern "C" 