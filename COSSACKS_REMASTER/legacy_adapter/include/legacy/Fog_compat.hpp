#pragma once

namespace legacy { namespace fog_compat {

// Load fog-related gradient/lookup tables for a given palette set index (0,1,2...)
// In the remaster, this is a no-op placeholder that ensures cursor pack selection
// mirrors the original behavior by mapping set -> cursor pack.
void LoadFog(int set);

} } // namespace legacy::fog_compat



