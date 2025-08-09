#pragma once

#include <memory>
#include <string>
#include <vector>

namespace resource_io {

class GSCArchive;

void initialize();

// Configure data root (default: "MAIN_GAME_DATA"). Relative to current working directory.
void set_data_root(const std::string& path);

// Opens typical archives from data root: resources.gsc (and others if present)
void open_default_archives();

// Read a file by name, searching archives first; if not found, tries data root, then raw path
bool read_file_anywhere(const std::string& name, std::vector<unsigned char>& out);

// Utility: find first entry in loaded archives that ends with suffix (case-insensitive)
bool try_find_entry_by_suffix(const std::string& suffix, std::string& outName);

// Convenience: initialize submodules (e.g., palettes, color palette)
void initialize_subsystems();

}  // namespace resource_io
