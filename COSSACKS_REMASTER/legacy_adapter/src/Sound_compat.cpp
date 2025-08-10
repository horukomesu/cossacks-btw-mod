#include "legacy/Sound_compat.hpp"

#include "audio_core/AudioCore.hpp"
#include "resource_io/ResourceIO.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <random>
#include <iostream>

namespace legacy { namespace sound_compat {

namespace {
struct Group {
    std::vector<std::string> filePaths; // paths to WAV files
    std::vector<unsigned int> buffers;  // OpenAL buffers (lazy-filled)
};

std::unordered_map<std::string, Group> g_groups; // key: uppercase group name
std::mt19937 g_rng{std::random_device{}()};

static std::string to_upper(const std::string& s) {
    std::string r = s;
    for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return r;
}

bool parse_soundlist_bytes(const std::vector<unsigned char>& bytes) {
    const char* data = reinterpret_cast<const char*>(bytes.data());
    const size_t n = bytes.size();
    size_t i = 0;
    while (i < n) {
        // Read group header: NAME count
        // Skip empty lines and comments
        while (i < n && (data[i] == '\r' || data[i] == '\n')) ++i;
        if (i >= n) break;
        if (data[i] == '/') { // comment line
            while (i < n && data[i] != '\n' && data[i] != '\r') ++i;
            continue;
        }
        // Read token
        std::string name;
        while (i < n && !std::isspace(static_cast<unsigned char>(data[i]))) name.push_back(data[i++]);
        while (i < n && std::isspace(static_cast<unsigned char>(data[i]))) ++i;
        if (name.empty()) { // skip rest of line
            while (i < n && data[i] != '\n' && data[i] != '\r') ++i;
            continue;
        }
        // Read count
        std::string countStr;
        while (i < n && !std::isspace(static_cast<unsigned char>(data[i]))) countStr.push_back(data[i++]);
        while (i < n && (data[i] == '\r' || data[i] == '\n')) ++i;
        int count = 0;
        try { count = std::stoi(countStr); } catch (...) { count = 0; }
        if (count <= 0) continue;

        Group grp;
        grp.filePaths.reserve(static_cast<size_t>(count));
        grp.buffers.resize(static_cast<size_t>(count), 0);
        for (int k = 0; k < count && i < n; ) {
            // Read next non-empty, non-comment token (path or DANGER/volume which we ignore)
            // We accept tokens per line; if token is DANGER or a number, we skip
            // If token is a path, store it
            // Move to end of line afterwards
            size_t lineStart = i;
            while (i < n && data[i] != '\n' && data[i] != '\r') ++i;
            std::string line(data + lineStart, i - lineStart);
            while (i < n && (data[i] == '\r' || data[i] == '\n')) ++i;

            // Trim
            auto ltrim = [](std::string& s){ size_t p = 0; while (p < s.size() && std::isspace(static_cast<unsigned char>(s[p]))) ++p; s.erase(0, p); };
            auto rtrim = [](std::string& s){ while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back(); };
            ltrim(line); rtrim(line);
            if (line.empty() || line[0] == '/') continue;
            if (line == "DANGER") continue;
            // If line has two tokens (path and volume), take the first token
            size_t sp = line.find_first_of("\t ");
            std::string path = (sp == std::string::npos) ? line : line.substr(0, sp);
            if (!path.empty()) {
                grp.filePaths.emplace_back(path);
                ++k;
            }
        }
        if (!grp.filePaths.empty()) {
            g_groups[to_upper(name)] = std::move(grp);
        }
    }
    return true;
}

unsigned int load_buffer_index(Group& grp, size_t idx) {
    if (idx >= grp.filePaths.size()) return 0;
    if (grp.buffers[idx] != 0) return grp.buffers[idx];
    std::vector<unsigned char> bytes;
    std::string p = grp.filePaths[idx];
    if (!resource_io::read_file_anywhere(p, bytes)) {
        if (!resource_io::read_file_anywhere(std::string("sounds\\") + p, bytes) &&
            !resource_io::read_file_anywhere(std::string("Sound\\") + p, bytes)) {
            if (!resource_io::read_file_anywhere(p + std::string(".wav"), bytes) &&
                !resource_io::read_file_anywhere(std::string("sounds\\") + p + ".wav", bytes) &&
                !resource_io::read_file_anywhere(std::string("Sound\\") + p + ".wav", bytes)) {
                return 0;
            }
        }
    }
    unsigned int buf = audio_core::create_buffer_from_wav_bytes(bytes.data(), bytes.size());
    grp.buffers[idx] = buf;
    return buf;
}

} // namespace

bool initialize() {
    g_groups.clear();
    std::vector<unsigned char> bytes;
    if (!resource_io::read_file_anywhere("sounds\\SOUNDLIST.TXT", bytes) &&
        !resource_io::read_file_anywhere("SOUNDLIST.TXT", bytes) &&
        !resource_io::read_file_anywhere("Resources\\SOUNDLIST.TXT", bytes)) {
        // try by suffix search inside archives
        std::string found;
        if (!resource_io::try_find_entry_by_suffix("SOUNDLIST.TXT", found)) {
            std::cerr << "[sound_compat] SOUNDLIST.TXT not found" << std::endl;
            return false;
        }
        if (!resource_io::read_file_anywhere(found, bytes)) {
            std::cerr << "[sound_compat] failed to read " << found << std::endl;
            return false;
        }
    }
    parse_soundlist_bytes(bytes);
    std::cout << "[sound_compat] groups loaded: " << g_groups.size() << std::endl;
    return true;
}

void shutdown() {
    // OpenAL buffers are freed when the process ends; no explicit buffer deletion cache here
    g_groups.clear();
}

unsigned int get_random_buffer(const std::string& groupName) {
    if (groupName.empty()) return 0;
    const std::string key = to_upper(groupName);
    auto it = g_groups.find(key);
    if (it == g_groups.end()) return 0;
    Group& grp = it->second;
    if (grp.filePaths.empty()) return 0;
    std::uniform_int_distribution<size_t> dist(0, grp.filePaths.size() - 1);
    const size_t idx = dist(g_rng);
    return load_buffer_index(grp, idx);
}

bool play_group(const std::string& groupName, float gain, float panX) {
    unsigned int buf = get_random_buffer(groupName);
    if (!buf) return false;
    return audio_core::play_buffer(buf, gain, panX) != 0;
}

} } // namespace legacy::sound_compat


