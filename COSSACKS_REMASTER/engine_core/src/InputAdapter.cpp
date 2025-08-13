#include "engine_core/InputAdapter.hpp"

#include <array>
#include <atomic>
#include <mutex>

namespace engine_core::input {

namespace {
constexpr size_t kQueueSize = 1024;
std::array<MouseEvent, kQueueSize> gQueue{};
std::atomic<size_t> gHead{0};
std::atomic<size_t> gTail{0};
std::mutex gQueueMutex;

int32_t gMouseX = 0;
int32_t gMouseY = 0;
bool gLeftPressed = false;
bool gRightPressed = false;

std::array<bool, 512> gKeyState{};
// Simple char input queue (UTF-32 codepoints)
std::array<uint32_t, kQueueSize> gCharQueue{};
std::atomic<size_t> gCharHead{0};
std::atomic<size_t> gCharTail{0};
} // namespace

void initialize() {
    gHead = 0;
    gTail = 0;
    gQueue.fill({});
    gCharHead = 0;
    gCharTail = 0;
    gCharQueue.fill(0);
    gKeyState.fill(false);
    gMouseX = gMouseY = 0;
    gLeftPressed = gRightPressed = false;
}

void SetMPtr(int32_t x, int32_t y) {
    gMouseX = x;
    gMouseY = y;
}

void AddMouseEvent(MouseEventType type, int32_t x, int32_t y, int32_t param) {
    std::lock_guard<std::mutex> lock(gQueueMutex);
    const size_t nextHead = (gHead + 1) % kQueueSize;
    if (nextHead == gTail) {
        // queue full, drop oldest
        gTail = (gTail + 1) % kQueueSize;
    }
    gQueue[gHead] = MouseEvent{type, x, y, param};
    gHead = nextHead;
}

bool ReadMEvent(MouseEvent& outEvent) {
    std::lock_guard<std::mutex> lock(gQueueMutex);
    if (gHead == gTail) return false;
    outEvent = gQueue[gTail];
    gTail = (gTail + 1) % kQueueSize;
    return true;
}

void SetKeyState(int32_t key, bool pressed) {
    if (key >= 0 && static_cast<size_t>(key) < gKeyState.size()) {
        gKeyState[static_cast<size_t>(key)] = pressed;
    }
}

bool IsKeyPressed(int32_t key) {
    if (key >= 0 && static_cast<size_t>(key) < gKeyState.size()) {
        return gKeyState[static_cast<size_t>(key)];
    }
    return false;
}

void SetLeftPressed(bool pressed) { gLeftPressed = pressed; }
void SetRightPressed(bool pressed) { gRightPressed = pressed; }

bool GetLeftPressed() { return gLeftPressed; }
bool GetRightPressed() { return gRightPressed; }

void GetPointer(int32_t& outX, int32_t& outY) {
    outX = gMouseX;
    outY = gMouseY;
}

void AddChar(uint32_t codepoint) {
    std::lock_guard<std::mutex> lock(gQueueMutex);
    const size_t next = (gCharHead + 1) % kQueueSize;
    if (next == gCharTail) {
        gCharTail = (gCharTail + 1) % kQueueSize;
    }
    gCharQueue[gCharHead] = codepoint;
    gCharHead = next;
}

bool ReadChar(uint32_t& outCodepoint) {
    std::lock_guard<std::mutex> lock(gQueueMutex);
    if (gCharHead == gCharTail) return false;
    outCodepoint = gCharQueue[gCharTail];
    gCharTail = (gCharTail + 1) % kQueueSize;
    return true;
}

} // namespace engine_core::input
