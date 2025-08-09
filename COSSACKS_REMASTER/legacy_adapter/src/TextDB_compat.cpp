#include "../include/legacy/TextDB_compat.hpp"

#include "resource_io/ResourceIO.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace legacy { namespace textdb {

namespace {
std::string g_lang{"eng"};
std::vector<std::string> g_extraPaths;
std::unordered_map<std::string, std::string> g_dict; // ID -> value
std::mutex g_mutex;

static inline std::string trim(std::string_view sv) {
    size_t b = 0;
    size_t e = sv.size();
    while (b < e && std::isspace(static_cast<unsigned char>(sv[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(sv[e - 1]))) --e;
    return std::string(sv.substr(b, e - b));
}

void parse_ini_like(const std::vector<unsigned char>& bytes) {
    const char* data = reinterpret_cast<const char*>(bytes.data());
    const size_t n = bytes.size();
    size_t i = 0;
    while (i < n) {
        size_t lineStart = i;
        while (i < n && data[i] != '\n' && data[i] != '\r') ++i;
        std::string_view line(data + lineStart, i - lineStart);
        // skip newline sequence
        while (i < n && (data[i] == '\n' || data[i] == '\r')) ++i;

        std::string s = trim(line);
        if (s.empty()) continue;
        if (s[0] == ';' || s[0] == '#') continue;
        // expected format: KEY=VALUE
        const size_t eq = s.find('=');
        if (eq == std::string::npos) continue;
        std::string key = trim(std::string_view{s.data(), eq});
        std::string val = trim(std::string_view{s.data() + eq + 1, s.size() - eq - 1});
        // strip optional quotes
        if (val.size() >= 2 && ((val.front() == '"' && val.back() == '"') || (val.front() == '\'' && val.back() == '\''))) {
            val = val.substr(1, val.size() - 2);
        }
        if (!key.empty()) {
            g_dict[key] = val;
        }
    }
}

bool try_load_path(const std::string& relOrName) {
    std::vector<unsigned char> bytes;
    if (!resource_io::read_file_anywhere(relOrName, bytes)) {
        // try by suffix search
        std::string found;
        if (!resource_io::try_find_entry_by_suffix(relOrName, found)) return false;
        if (!resource_io::read_file_anywhere(found, bytes)) return false;
    }
    parse_ini_like(bytes);
    return true;
}
} // namespace

void set_language(const std::string& lang3) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (lang3.size() == 3) g_lang = lang3;
}

void add_dictionary_path(const std::string& path) {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_extraPaths.push_back(path);
}

bool initialize() {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_dict.clear();
    // typical names (examples): "text\\eng\menu.ini", "text\\eng\strings.ini" etc.
    // Load any present; order defines priority (later wins)
    const std::vector<std::string> defaults = {
        std::string("text\\") + g_lang + "\\menu.ini",
        std::string("text\\") + g_lang + "\\strings.ini",
        std::string("Interface\\text_") + g_lang + ".ini",
    };
    for (const auto& p : defaults) {
        try_load_path(p);
    }
    for (const auto& p : g_extraPaths) {
        try_load_path(p);
    }
    return true;
}

const char* get_text_by_id(const char* id) {
    if (!id) return "";
    std::lock_guard<std::mutex> lock(g_mutex);
    auto it = g_dict.find(id);
    if (it != g_dict.end()) return it->second.c_str();
    return "";
}

int get_value_by_id(const char* id, int defaultValue) {
    if (!id) return defaultValue;
    std::lock_guard<std::mutex> lock(g_mutex);
    auto it = g_dict.find(id);
    if (it == g_dict.end()) return defaultValue;
    try {
        return std::stoi(it->second);
    } catch (...) {
        return defaultValue;
    }
}

} } // namespace legacy::textdb

extern "C" {
    const char* GetTextByID(const char* id) { return legacy::textdb::get_text_by_id(id); }
    const char* GETS(const char* id) { return legacy::textdb::get_text_by_id(id); }
    int GETV(const char* id) { return legacy::textdb::get_value_by_id(id, 0); }
}


