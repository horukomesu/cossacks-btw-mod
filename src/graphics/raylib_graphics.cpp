#include "raylib_graphics.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Global graphics instance
RaylibGraphics* g_graphics = nullptr;

RaylibGraphics::RaylibGraphics()
    : screen_width(800)
    , screen_height(600)
    , fullscreen(false)
    , vsync_enabled(true)
    , camera_enabled(false)
{
    // Initialize camera
    camera.target = { 0.0f, 0.0f };
    camera.offset = { 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

RaylibGraphics::~RaylibGraphics()
{
    Shutdown();
}

bool RaylibGraphics::Initialize(int width, int height, bool fs)
{
    screen_width = width;
    screen_height = height;
    fullscreen = fs;
    
    try
    {
        // Raylib window is initialized in main
        // Here we just set up our internal state
        
        // Initialize default palette
        std::vector<Color> default_palette(256);
        for (int i = 0; i < 256; i++)
        {
            default_palette[i] = { (unsigned char)i, (unsigned char)i, (unsigned char)i, 255 };
        }
        palettes["default"] = default_palette;
        
        last_error = "";
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Failed to initialize graphics: " + std::string(e.what()));
        return false;
    }
}

void RaylibGraphics::Shutdown()
{
    // Unload all textures
    for (auto& texture : textures)
    {
        UnloadTexture(texture.second);
    }
    textures.clear();
    
    // Destroy all render textures
    for (auto& render_texture : render_textures)
    {
        UnloadRenderTexture(render_texture.second);
    }
    render_textures.clear();
    
    // Clear palettes
    palettes.clear();
    
    // Clear sprite batch
    sprite_batch.clear();
}

void RaylibGraphics::SetScreenSize(int width, int height)
{
    screen_width = width;
    screen_height = height;
    SetWindowSize(width, height);
}

void RaylibGraphics::SetFullscreen(bool fs)
{
    fullscreen = fs;
    if (fs)
    {
        if (!IsWindowFullscreen())
            ToggleFullscreen();
    }
    else
    {
        if (IsWindowFullscreen())
            ToggleFullscreen();
    }
}

void RaylibGraphics::SetVSync(bool enabled)
{
    vsync_enabled = enabled;
    // Raylib handles VSync through SetTargetFPS
}

bool RaylibGraphics::LoadTexture(const std::string& name, const std::string& filepath)
{
    try
    {
        Texture2D texture = LoadTexture(filepath.c_str());
        if (texture.id == 0)
        {
            LogError("Failed to load texture: " + filepath);
            return false;
        }
        
        // Unload existing texture if it exists
        if (textures.find(name) != textures.end())
        {
            UnloadTexture(textures[name]);
        }
        
        textures[name] = texture;
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Exception loading texture " + filepath + ": " + e.what());
        return false;
    }
}

bool RaylibGraphics::LoadTextureFromMemory(const std::string& name, const unsigned char* data, int size)
{
    try
    {
        // Create a temporary file for raylib to load from
        // This is a workaround since raylib doesn't have direct memory loading for all formats
        std::string temp_file = "temp_" + name + ".tmp";
        std::ofstream file(temp_file, std::ios::binary);
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
        
        bool result = LoadTexture(name, temp_file);
        
        // Clean up temporary file
        std::remove(temp_file.c_str());
        
        return result;
    }
    catch (const std::exception& e)
    {
        LogError("Exception loading texture from memory: " + std::string(e.what()));
        return false;
    }
}

void RaylibGraphics::UnloadTexture(const std::string& name)
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        UnloadTexture(it->second);
        textures.erase(it);
    }
}

Texture2D* RaylibGraphics::GetTexture(const std::string& name)
{
    auto it = textures.find(name);
    return (it != textures.end()) ? &it->second : nullptr;
}

bool RaylibGraphics::CreateRenderTexture(const std::string& name, int width, int height)
{
    try
    {
        RenderTexture2D render_texture = LoadRenderTexture(width, height);
        if (render_texture.id == 0)
        {
            LogError("Failed to create render texture: " + name);
            return false;
        }
        
        // Unload existing render texture if it exists
        if (render_textures.find(name) != render_textures.end())
        {
            UnloadRenderTexture(render_textures[name]);
        }
        
        render_textures[name] = render_texture;
        return true;
    }
    catch (const std::exception& e)
    {
        LogError("Exception creating render texture " + name + ": " + e.what());
        return false;
    }
}

void RaylibGraphics::DestroyRenderTexture(const std::string& name)
{
    auto it = render_textures.find(name);
    if (it != render_textures.end())
    {
        UnloadRenderTexture(it->second);
        render_textures.erase(it);
    }
}

RenderTexture2D* RaylibGraphics::GetRenderTexture(const std::string& name)
{
    auto it = render_textures.find(name);
    return (it != render_textures.end()) ? &it->second : nullptr;
}

void RaylibGraphics::BeginDrawing()
{
    ::BeginDrawing();
}

void RaylibGraphics::EndDrawing()
{
    ::EndDrawing();
}

void RaylibGraphics::Clear(Color color)
{
    ClearBackground(color);
}

void RaylibGraphics::DrawSprite(const std::string& texture_name, int x, int y)
{
    Texture2D* texture = GetTexture(texture_name);
    if (texture)
    {
        DrawTexture(*texture, x, y, WHITE);
    }
}

void RaylibGraphics::DrawSprite(const std::string& texture_name, int x, int y, Rectangle src)
{
    Texture2D* texture = GetTexture(texture_name);
    if (texture)
    {
        Rectangle dest = { (float)x, (float)y, src.width, src.height };
        DrawTexturePro(*texture, src, dest, { 0, 0 }, 0.0f, WHITE);
    }
}

void RaylibGraphics::DrawSprite(const std::string& texture_name, int x, int y, Rectangle src, Color tint)
{
    Texture2D* texture = GetTexture(texture_name);
    if (texture)
    {
        Rectangle dest = { (float)x, (float)y, src.width, src.height };
        DrawTexturePro(*texture, src, dest, { 0, 0 }, 0.0f, tint);
    }
}

void RaylibGraphics::DrawSprite(const std::string& texture_name, int x, int y, Rectangle src, Color tint, float rotation, Vector2 origin, float scale)
{
    Texture2D* texture = GetTexture(texture_name);
    if (texture)
    {
        Rectangle dest = { (float)x, (float)y, src.width * scale, src.height * scale };
        DrawTexturePro(*texture, src, dest, origin, rotation, tint);
    }
}

void RaylibGraphics::DrawRectangle(int x, int y, int width, int height, Color color)
{
    ::DrawRectangle(x, y, width, height, color);
}

void RaylibGraphics::DrawRectangleLines(int x, int y, int width, int height, Color color)
{
    ::DrawRectangleLines(x, y, width, height, color);
}

void RaylibGraphics::DrawCircle(int x, int y, float radius, Color color)
{
    ::DrawCircle(x, y, radius, color);
}

void RaylibGraphics::DrawLine(int x1, int y1, int x2, int y2, Color color)
{
    ::DrawLine(x1, y1, x2, y2, color);
}

void RaylibGraphics::DrawPixel(int x, int y, Color color)
{
    ::DrawPixel(x, y, color);
}

void RaylibGraphics::DrawText(const std::string& text, int x, int y, int size, Color color)
{
    ::DrawText(text.c_str(), x, y, size, color);
}

void RaylibGraphics::DrawTextEx(Font font, const std::string& text, Vector2 position, float fontSize, float spacing, Color tint)
{
    ::DrawTextEx(font, text.c_str(), position, fontSize, spacing, tint);
}

void RaylibGraphics::SetCamera(Camera2D cam)
{
    camera = cam;
}

Camera2D RaylibGraphics::GetCamera() const
{
    return camera;
}

void RaylibGraphics::EnableCamera()
{
    camera_enabled = true;
}

void RaylibGraphics::DisableCamera()
{
    camera_enabled = false;
}

bool RaylibGraphics::LoadPalette(const std::string& name, const std::string& filepath)
{
    // TODO: Implement palette loading from file
    // This would typically load a .pal file or similar
    LogError("Palette loading from file not yet implemented");
    return false;
}

bool RaylibGraphics::LoadPaletteFromMemory(const std::string& name, const unsigned char* data, int size)
{
    // TODO: Implement palette loading from memory
    // This would parse palette data format
    LogError("Palette loading from memory not yet implemented");
    return false;
}

void RaylibGraphics::SetPalette(const std::string& name)
{
    // TODO: Set active palette
    // This would affect subsequent drawing operations
}

Color RaylibGraphics::GetPaletteColor(const std::string& palette_name, int index)
{
    auto it = palettes.find(palette_name);
    if (it != palettes.end() && index >= 0 && index < it->second.size())
    {
        return it->second[index];
    }
    return BLACK;
}

Vector2 RaylibGraphics::GetMousePosition()
{
    return ::GetMousePosition();
}

bool RaylibGraphics::IsKeyPressed(int key)
{
    return ::IsKeyPressed(key);
}

bool RaylibGraphics::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

bool RaylibGraphics::IsMouseButtonPressed(int button)
{
    return ::IsMouseButtonPressed(button);
}

bool RaylibGraphics::IsMouseButtonDown(int button)
{
    return ::IsMouseButtonDown(button);
}

bool RaylibGraphics::SaveScreenshot(const std::string& filename)
{
    TakeScreenshot(filename.c_str());
    return true; // raylib doesn't return success/failure
}

void RaylibGraphics::LockSurface()
{
    // No-op for raylib compatibility
}

void RaylibGraphics::UnlockSurface()
{
    // No-op for raylib compatibility
}

void RaylibGraphics::FlipBuffers()
{
    // No-op for raylib compatibility (handled by EndDrawing)
}

void RaylibGraphics::BeginSpriteBatch()
{
    sprite_batch.clear();
}

void RaylibGraphics::EndSpriteBatch()
{
    // Batch is automatically drawn when full or when EndSpriteBatch is called
}

void RaylibGraphics::AddToBatch(const std::string& texture_name, Rectangle src, Rectangle dest)
{
    // TODO: Implement sprite batching for performance
    // For now, just draw immediately
    DrawSprite(texture_name, (int)dest.x, (int)dest.y, src);
}

void RaylibGraphics::DrawBatch()
{
    // TODO: Implement batch drawing
    sprite_batch.clear();
}

std::string RaylibGraphics::GetLastError() const
{
    return last_error;
}

void RaylibGraphics::LogError(const std::string& error)
{
    last_error = error;
    std::cerr << "RaylibGraphics Error: " << error << std::endl;
}

// C-style interface implementations
extern "C" {

void InitRaylibGraphics()
{
    if (!g_graphics)
    {
        g_graphics = new RaylibGraphics();
        g_graphics->Initialize(800, 600, false);
    }
}

void CleanupRaylibGraphics()
{
    if (g_graphics)
    {
        delete g_graphics;
        g_graphics = nullptr;
    }
}

void DrawRaylibGraphics()
{
    if (g_graphics)
    {
        // Main drawing is handled elsewhere
        // This is for any additional graphics processing
    }
}

void ClearRaylibScreen()
{
    if (g_graphics)
    {
        g_graphics->Clear(BLACK);
    }
}

bool LoadRaylibTexture(const char* name, const char* filepath)
{
    return g_graphics ? g_graphics->LoadTexture(name, filepath) : false;
}

void UnloadRaylibTexture(const char* name)
{
    if (g_graphics)
    {
        g_graphics->UnloadTexture(name);
    }
}

void DrawRaylibSprite(const char* texture_name, int x, int y)
{
    if (g_graphics)
    {
        g_graphics->DrawSprite(texture_name, x, y);
    }
}

void DrawRaylibSpriteEx(const char* texture_name, int x, int y, int src_x, int src_y, int src_w, int src_h)
{
    if (g_graphics)
    {
        Rectangle src = { (float)src_x, (float)src_y, (float)src_w, (float)src_h };
        g_graphics->DrawSprite(texture_name, x, y, src);
    }
}

void DrawRaylibRectangle(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (g_graphics)
    {
        Color color = { r, g, b, a };
        g_graphics->DrawRectangle(x, y, width, height, color);
    }
}

void DrawRaylibLine(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (g_graphics)
    {
        Color color = { r, g, b, a };
        g_graphics->DrawLine(x1, y1, x2, y2, color);
    }
}

void DrawRaylibPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (g_graphics)
    {
        Color color = { r, g, b, a };
        g_graphics->DrawPixel(x, y, color);
    }
}

void DrawRaylibText(const char* text, int x, int y, int size, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (g_graphics)
    {
        Color color = { r, g, b, a };
        g_graphics->DrawText(text, x, y, size, color);
    }
}

void LockRaylibSurface()
{
    if (g_graphics)
    {
        g_graphics->LockSurface();
    }
}

void UnlockRaylibSurface()
{
    if (g_graphics)
    {
        g_graphics->UnlockSurface();
    }
}

void FlipRaylibBuffers()
{
    if (g_graphics)
    {
        g_graphics->FlipBuffers();
    }
}

void SetRaylibScreenSize(int width, int height)
{
    if (g_graphics)
    {
        g_graphics->SetScreenSize(width, height);
    }
}

void SetRaylibFullscreen(bool fullscreen)
{
    if (g_graphics)
    {
        g_graphics->SetFullscreen(fullscreen);
    }
}

void* GetRaylibScreenPtr()
{
    // For compatibility - return a dummy pointer
    // In the original DirectDraw, this would return a pointer to video memory
    // With raylib, we work with textures, so we return a dummy pointer
    static char dummy_screen[1024 * 768];
    return dummy_screen;
}

void FlipRaylibPages()
{
    FlipRaylibBuffers();
}

void LoadRaylibPalette(const char* filename, void* palette_entries)
{
    // Load palette and copy to the provided buffer
    if (g_graphics && filename && palette_entries)
    {
        g_graphics->LoadPalette("current", filename);
        // Copy palette data to the provided buffer
        // This is a simplified implementation - in a real implementation
        // you'd read the palette file and populate the PALETTEENTRY structure
        memset(palette_entries, 0, 256 * 4); // Clear palette entries
    }
}

bool InitializeRaylibGraphics(int width, int height, const char* title, bool fullscreen)
{
    if (!g_graphics)
    {
        g_graphics = new RaylibGraphics();
    }
    return g_graphics->Initialize(width, height, fullscreen);
}

} // extern "C"

// Static function implementations for RaylibGraphics class
bool RaylibGraphics::Initialize(int width, int height, const char* title, bool fullscreen)
{
    if (!g_graphics)
    {
        g_graphics = new RaylibGraphics();
    }
    return g_graphics->Initialize(width, height, fullscreen);
}

void RaylibGraphics::FlipPages()
{
    FlipRaylibBuffers();
}

void* RaylibGraphics::GetScreenPtr()
{
    return GetRaylibScreenPtr();
}

void RaylibGraphics::LoadPalette(const char* filename, void* palette_entries)
{
    LoadRaylibPalette(filename, palette_entries);
}

bool RaylibGraphics::IsInitialized()
{
    return g_graphics != nullptr;
}

void RaylibGraphics::ClearScreen()
{
    ClearRaylibScreen();
} 