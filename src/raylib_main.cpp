#include <raylib.h>
#include <iostream>
#include <string>

// Game includes
#include "graphics/raylib_graphics.h"
#include "audio/raylib_audio.h"
#include "input/raylib_input.h"
#include "utils/raylib_utils.h"

// Original game includes (converted)
#include "MapDiscr.h"
#include "Nation.h"
#include "Megapolis.h"
#include "NewMon.h"
#include "menu.h"
#include "fog.h"
#include "ActiveScenary.h"
#include "Safety.h"

// Global variables
extern bool window_mode;
extern int screen_width;
extern int screen_height;
extern double screen_ratio;

// Game state
bool GameExit = false;
bool GameNeedToDraw = false;
bool InGame = false;
bool MakeMenu = false;
bool bActive = true;

// Initialize global variables
bool window_mode = false;
int screen_width = 1024;
int screen_height = 768;
double screen_ratio = 1.0;

// Forward declarations
void InitGame();
void LoadGameResources();
void GameLoop();
void CleanupGame();
void PreDrawGameProcess();
void DrawAllScreen();
void ProcessMessages();
void FastScreenProcess();
void PostDrawGameProcess();

int main(int argc, char* argv[])
{
    // Parse command line arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "/window" || arg == "-window")
        {
            window_mode = true;
        }
        else if (arg == "/borderless" || arg == "-borderless")
        {
            window_mode = true;
            // Will be handled by raylib window flags
        }
        else if (arg.find("/width=") == 0 || arg.find("-width=") == 0)
        {
            screen_width = std::stoi(arg.substr(arg.find('=') + 1));
        }
        else if (arg.find("/height=") == 0 || arg.find("-height=") == 0)
        {
            screen_height = std::stoi(arg.substr(arg.find('=') + 1));
        }
    }

    // Calculate screen ratio
    screen_ratio = (double)screen_width / (double)screen_height;

    // Initialize raylib
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    if (window_mode)
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    }
    else
    {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }

    InitWindow(screen_width, screen_height, "Cossacks: Back to War (Raylib)");
    SetTargetFPS(60);

    // Initialize game systems
    try
    {
        InitRaylibGraphics();
        InitRaylibAudio();
        InitRaylibInput();
        InitRaylibUtils();
        
        // Initialize original game systems
        InitGame();
        LoadGameResources();
        
        std::cout << "Game initialized successfully!" << std::endl;
        
        // Main game loop
        GameLoop();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Game initialization failed: " << e.what() << std::endl;
        CloseWindow();
        return 1;
    }

    // Cleanup
    CleanupGame();
    CloseWindow();
    return 0;
}

void GameLoop()
{
    while (!WindowShouldClose() && !GameExit)
    {
        // Update input
        UpdateRaylibInput();
        
        // Process game logic
        if (bActive)
        {
            PreDrawGameProcess();
            ProcessMessages();
            
            if (GameNeedToDraw)
            {
                DrawAllScreen();
                GameNeedToDraw = false;
            }
            else
            {
                FastScreenProcess();
            }
            
            PostDrawGameProcess();
        }
        
        // Begin drawing
        BeginDrawing();
        
        // Clear screen
        ClearBackground(BLACK);
        
        // Draw game
        DrawRaylibGraphics();
        
        // Draw UI
        DrawText("Cossacks: Back to War (Raylib)", 10, 10, 20, WHITE);
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 35, 20, WHITE);
        
        EndDrawing();
    }
}

void InitGame()
{
    // Initialize game state
    GameExit = false;
    GameNeedToDraw = true;
    InGame = false;
    MakeMenu = true;
    
    // TODO: Initialize original game systems
    // This will be filled in as we migrate the original initialization code
    
    std::cout << "Game state initialized" << std::endl;
}

void LoadGameResources()
{
    // TODO: Load game resources using raylib
    // This will replace the original resource loading system
    
    std::cout << "Game resources loaded" << std::endl;
}

void CleanupGame()
{
    // Cleanup raylib systems
    CleanupRaylibGraphics();
    CleanupRaylibAudio();
    CleanupRaylibInput();
    CleanupRaylibUtils();
    
    // TODO: Cleanup original game systems
    
    std::cout << "Game cleanup completed" << std::endl;
}

// Stub implementations for original functions
void PreDrawGameProcess()
{
    // TODO: Implement original pre-draw game processing
    // This will be migrated from the original Ddex1.cpp
}

void DrawAllScreen()
{
    // TODO: Implement screen drawing
    // This will be migrated from the original drawing system
}

void ProcessMessages()
{
    // TODO: Process game messages
    // This will be migrated from the original message processing
}

void FastScreenProcess()
{
    // TODO: Implement fast screen processing
    // This will be migrated from the original system
}

void PostDrawGameProcess()
{
    // TODO: Implement post-draw game processing
    // This will be migrated from the original system
} 