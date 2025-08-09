#pragma once

#include <cstdint>
#include <string>

namespace legacy { namespace textdb {

// Configure language code (e.g., "eng", "rus"). Default: "eng"
void set_language(const std::string& lang3);

// Optional: add custom dictionary file path (archive-relative or direct), loaded on initialize()
void add_dictionary_path(const std::string& path);

// Load default dictionaries and any added paths. Safe to call multiple times.
bool initialize();

// Lookup functions
const char* get_text_by_id(const char* id);
int get_value_by_id(const char* id, int defaultValue = 0);

} } // namespace legacy::textdb

// Legacy-free shims expected by original code
extern "C" {
    const char* GetTextByID(const char* id);
    const char* GETS(const char* id);
    int GETV(const char* id);
}


