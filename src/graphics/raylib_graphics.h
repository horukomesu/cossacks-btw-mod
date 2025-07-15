#pragma once

#include <raylib.h>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

// Forward declarations
struct RaylibTexture;
struct RaylibSurface;
struct RaylibPalette;

// Use raylib's Rectangle type with namespace to avoid Windows conflicts
using RaylibRectangle = ::Rectangle;

// Raylib Graphics System
class RaylibGraphics
{
private:
    // Screen and rendering
    int screen_width;
    int screen_height;
    bool fullscreen;
    bool vsync_enabled;
    
    // Textures and surfaces
    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, RenderTexture2D> render_textures;
    
    // Drawing state
    Camera2D camera;
    bool camera_enabled;
    
    // Palette support (for original game compatibility)
    std::unordered_map<std::string, std::vector<Color>> palettes;
    
    // Sprite batching
    std::vector<RaylibRectangle> sprite_batch;
    
public:
    RaylibGraphics();
    ~RaylibGraphics();
    
    // Initialization
    bool Initialize(int width, int height, bool fullscreen);
    void Shutdown();
    
    // Screen management
    void SetScreenSize(int width, int height);
    void SetFullscreen(bool fullscreen);
    void SetVSync(bool enabled);
    
    // Texture management
    bool LoadTexture(const std::string& name, const std::string& filepath);
    bool LoadTextureFromMemory(const std::string& name, const unsigned char* data, int size);
    void UnloadTexture(const std::string& name);
    Texture2D* GetTexture(const std::string& name);
    
    // Render texture management
    bool CreateRenderTexture(const std::string& name, int width, int height);
    void DestroyRenderTexture(const std::string& name);
    RenderTexture2D* GetRenderTexture(const std::string& name);
    
    // Drawing functions
    void BeginDrawing();
    void EndDrawing();
    void Clear(Color color = BLACK);
    
    // Sprite drawing
    void DrawSprite(const std::string& texture_name, int x, int y);
    void DrawSprite(const std::string& texture_name, int x, int y, RaylibRectangle src);
    void DrawSprite(const std::string& texture_name, int x, int y, RaylibRectangle src, Color tint);
    void DrawSprite(const std::string& texture_name, int x, int y, RaylibRectangle src, Color tint, float rotation, Vector2 origin, float scale);
    
    // Primitive drawing
    void DrawRectangle(RaylibRectangle rect, Color color);
    void DrawRectangleLines(int x, int y, int width, int height, Color color);
    void DrawCircle(int x, int y, float radius, Color color);
    void DrawLine(int x1, int y1, int x2, int y2, Color color);
    void DrawPixel(int x, int y, Color color);
    
    // Text drawing
    void DrawText(const std::string& text, int x, int y, int size, Color color);
    void DrawTextEx(Font font, const std::string& text, Vector2 position, float fontSize, float spacing, Color tint);
    
    // Camera
    void SetCamera(Camera2D camera);
    Camera2D GetCamera() const;
    void EnableCamera();
    void DisableCamera();
    
    // Palette support (for original game compatibility)
    bool LoadPalette(const std::string& name, const std::string& filepath);
    bool LoadPaletteFromMemory(const std::string& name, const unsigned char* data, int size);
    void SetPalette(const std::string& name);
    Color GetPaletteColor(const std::string& palette_name, int index);
    
    // Utility functions
    Vector2 GetMousePosition();
    bool IsKeyPressed(int key);
    bool IsKeyDown(int key);
    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonDown(int button);
    
    // Screen capture
    bool SaveScreenshot(const std::string& filename);
    
    // Surface operations (for compatibility with original surface-based operations)
    void LockSurface();
    void UnlockSurface();
    void FlipBuffers();
    
    // Batch operations
    void BeginSpriteBatch();
    void EndSpriteBatch();
    void AddToBatch(const std::string& texture_name, RaylibRectangle src, RaylibRectangle dest);
    void DrawBatch();
    
    // Error handling
    std::string GetLastError() const;
    
    // DirectDraw compatibility functions
    static bool Initialize(int width, int height, const char* title, bool fullscreen);
    static void FlipPages();
    static void* GetScreenPtr();
    static void LoadPalette(const char* filename, void* palette_entries);
    static bool IsInitialized();
    static void ClearScreen();
    
private:
    std::string last_error;
    
    // Helper functions
    bool LoadTextureInternal(const std::string& name, const std::string& filepath);
    Color PaletteToColor(const std::vector<Color>& palette, int index);
    void LogError(const std::string& error);
};

// Global graphics instance
extern RaylibGraphics* g_graphics;

// C-style interface for compatibility with original code
extern "C" {
    // Initialization
    void InitRaylibGraphics();
    void CleanupRaylibGraphics();
    
    // Drawing
    void DrawRaylibGraphics();
    void ClearRaylibScreen();
    
    // Textures
    bool LoadRaylibTexture(const char* name, const char* filepath);
    void UnloadRaylibTexture(const char* name);
    
    // Sprites
    void DrawRaylibSprite(const char* texture_name, int x, int y);
    void DrawRaylibSpriteEx(const char* texture_name, int x, int y, int src_x, int src_y, int src_w, int src_h);
    
    // Primitives
    void DrawRaylibRectangle(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void DrawRaylibLine(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void DrawRaylibPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    
    // Text
    void DrawRaylibText(const char* text, int x, int y, int size, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    
    // Surface operations (compatibility)
    void LockRaylibSurface();
    void UnlockRaylibSurface();
    void FlipRaylibBuffers();
    
    // Screen
    void SetRaylibScreenSize(int width, int height);
    void SetRaylibFullscreen(bool fullscreen);
    
    // DirectDraw compatibility functions
    void* GetRaylibScreenPtr();
    void FlipRaylibPages();
    void LoadRaylibPalette(const char* filename, void* palette_entries);
    bool InitializeRaylibGraphics(int width, int height, const char* title, bool fullscreen);
}

// Compatibility macros for easier migration
#define RAYLIB_BLACK    (Color){0, 0, 0, 255}
#define RAYLIB_WHITE    (Color){255, 255, 255, 255}
#define RAYLIB_RED      (Color){255, 0, 0, 255}
#define RAYLIB_GREEN    (Color){0, 255, 0, 255}
#define RAYLIB_BLUE     (Color){0, 0, 255, 255}
#define RAYLIB_YELLOW   (Color){255, 255, 0, 255}
#define RAYLIB_MAGENTA  (Color){255, 0, 255, 255}
#define RAYLIB_CYAN     (Color){0, 255, 255, 255} 