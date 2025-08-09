#pragma once

#include <cstdint>

namespace legacy::fastdraw {

extern int WindX;
extern int WindY;
extern int WindLx;
extern int WindLy;
extern int WindX1;
extern int WindY1;
extern int ScrWidth;
extern int ScrHeight;

void SetRLCWindow(int x, int y, int lx, int ly, int slx);
void ClearScreen();

    // Legacy solid fill rectangle using 8-bit palette index color
    void CBar(int x, int y, int Lx, int Ly, uint8_t colorIndex);

    // Legacy blit from backbuffer to screen; in our GL path it's effectively a no-op
    void CopyToScreen(int x, int y, int Lx, int Ly);

} // namespace legacy::fastdraw
