// STUB VERSION - SINGLEPLAYER ONLY
// All multiplayer functions are stubbed out for singleplayer mode

#ifdef SINGLEPLAYER_ONLY

// Stub functions that do nothing
inline bool CreateMultiplaterInterface() { return false; }
inline void InitMultiDialogs() { }
inline void SetupMultiplayer(void* hInstance) { }
inline void ShutdownMultiplayer(bool Final) { }

#else

// Original multiplayer functions (not used in singleplayer)
bool CreateMultiplaterInterface();
void InitMultiDialogs();
void SetupMultiplayer(HINSTANCE hInstance);
void ShutdownMultiplayer(bool Final);

#endif