#pragma once

namespace legacy { namespace cursor_compat {

// Set current cursor GP pack (resource name), e.g., "Interface\\CURSOR"
void set_cursor_pack(const char* gp_name);

// Set current cursor frame index (matches curptr semantics in legacy)
void set_cursor_frame(int frameIndex);

// Draw cursor at current mouseX/mouseY each frame
void draw_cursor();

// Optional: query current cursor GPID (or -1 if not set)
int get_current_gpid();

} } // namespace legacy::cursor_compat


