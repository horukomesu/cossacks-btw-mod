#include "resource_io/ResourceIO.hpp"
#include "resource_io/Palettes.hpp"
#include "resource_io/ColorPalette.hpp"

#include "resource_io/Arc.hpp"
#include "resource_io/ResFile.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

namespace resource_io {

namespace {
std::vector<std::unique_ptr<GSCArchive>> g_archives;
fs::path g_data_root = fs::path("MAIN_GAME_DATA");

void try_open_archive(const fs::path& p) {
    std::error_code ec;
    if (!fs::exists(p, ec)) return;
    auto a = GSCArchive::open(p.string());
    if (a && a->is_open()) {
        std::cout << "[resource_io] opened archive: " << p.string() << " entries=" << a->list().size() << std::endl;
        g_archives.emplace_back(std::move(a));
    }
}

static std::string to_upper(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return static_cast<char>(std::toupper(c)); });
    return r;
}

} // namespace

void initialize() {
    std::cout << "[resource_io] initialized" << std::endl;
}

void set_data_root(const std::string& path) {
    g_data_root = fs::path(path);
}

void open_default_archives() {
    // Only resources.gsc (single archive) per original behavior
    try_open_archive(g_data_root / "resources.gsc");
}

bool read_file_anywhere(const std::string& name, std::vector<unsigned char>& out) {
    // Try archives in order
    for (const auto& arc : g_archives) {
        if (!arc) continue;
        if (arc->read_file(name, out)) return true;
        // Try normalized slashes
        std::string alt1 = name;
        for (char& c : alt1) if (c == '/') c = '\\';
        if (alt1 != name && arc->read_file(alt1, out)) return true;
        std::string alt2 = name;
        for (char& c : alt2) if (c == '\\') c = '/';
        if (alt2 != name && arc->read_file(alt2, out)) return true;
        // Try uppercase name (archives store uppercase entries)
        std::string up = alt1;
        for (char& c : up) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if (arc->read_file(up, out)) return true;
    }
    // Try file relative to data root
    {
        std::error_code ec;
        fs::path p = g_data_root / name;
        if (fs::exists(p, ec)) {
            out = ResFile::read_all(p.string());
            if (!out.empty()) return true;
        }
    }
    // Fall back to raw path
    out = ResFile::read_all(name);
    return !out.empty();
}

bool try_find_entry_by_suffix(const std::string& suffix, std::string& outName) {
    // Normalize to backslashes for archive names
    std::string norm = suffix;
    for (char& c : norm) if (c == '/') c = '\\';
    const std::string upSuf = to_upper(norm);
    for (const auto& arc : g_archives) {
        if (!arc) continue;
        for (const auto& e : arc->list()) {
            const std::string up = to_upper(e.nameUpper);
            if (up.size() >= upSuf.size() && up.compare(up.size() - upSuf.size(), upSuf.size(), upSuf) == 0) {
                outName = e.nameUpper; // original upper name
                return true;
            }
        }
    }
    // Fallback: check on-disk relative to data root only, then raw path
    std::error_code ec;
    fs::path p = g_data_root / norm;
    if (fs::exists(p, ec)) { outName = p.string(); return true; }
    if (fs::exists(norm, ec)) { outName = norm; return true; }
    return false;
}

void initialize_subsystems() {
    palettes::initialize();
    color_palette::initialize();
}

}  // namespace resource_io
