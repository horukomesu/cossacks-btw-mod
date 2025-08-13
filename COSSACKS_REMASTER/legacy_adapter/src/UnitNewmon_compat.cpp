#include "legacy/UnitNewmon_compat.hpp"

#include "resource_io/ResFile.hpp"
#include "resource_io/ResourceIO.hpp"
#include "legacy/GP_Draw_compat.hpp"

#include <algorithm>
#include <cctype>

namespace legacy { namespace newmon_compat {

static std::unordered_map<int, RLCRefEntry> g_rlc;
static std::unordered_map<std::string, UnitDef> g_units; // key uppercased

static std::string to_upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

bool load_weapon_ads() {
    if (!g_rlc.empty()) return true;
    std::vector<std::string> lines;
    if (!resource_io::resfile::read_text_lines("weapon.ads", lines)) return false;
    int lineNo = 0;
    for (const std::string& raw : lines) {
        ++lineNo;
        if (raw.empty()) continue;
        if (raw[0] == '/' || raw[0] == '[') continue;
        std::string s = raw;
        // tokenization by spaces
        std::vector<std::string> tok; tok.reserve(8);
        std::string cur;
        for (char ch : s) {
            if (std::isspace(static_cast<unsigned char>(ch))) { if (!cur.empty()) { tok.emplace_back(cur); cur.clear(); } }
            else cur.push_back(ch);
        }
        if (!cur.empty()) tok.emplace_back(cur);
        if (tok.empty()) continue;
        if (tok[0] == "USERLC" && tok.size() >= 5) {
            int p1 = std::stoi(tok[1]);
            std::string stem = tok[2];
            std::string kind = tok[3];
            int dx = std::stoi(tok[4]);
            int dy = (tok.size() >= 6) ? std::stoi(tok[5]) : 0;
            RLCRefEntry e; e.gpStem = to_upper(stem); e.dx = dx; e.dy = dy; e.isShadow = (to_upper(kind) == "SHADOW");
            g_rlc[p1] = e;
        }
        // USERLCEXT could be parsed here when needed (with extra params)
    }
    return true;
}

const std::unordered_map<int, RLCRefEntry>& get_rlc_table() { return g_rlc; }

bool load_unit_md(const std::string& name) {
    std::string base = name; for (char& c : base) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    if (g_units.find(base) != g_units.end()) return true;
    std::vector<std::string> lines;
    std::string path = base + ".MD";
    if (!resource_io::resfile::read_text_lines(path, lines)) return false;
    UnitDef def;
    // Temporary local RLC maps per MD file (emulating RLCRef[] index table expansion), starts empty; relies on weapon.ads having base refs if used by indices.
    // For simplicity, we will only support the common form with absolute ranges (the @ form in original) in this step.
    int lineNo = 0;
    AnimationDef* current = nullptr;
    for (const std::string& raw : lines) {
        ++lineNo;
        if (raw.empty()) continue;
        if (raw[0] == '/' || raw[0] == '[') continue;
        std::string s = raw;
        std::vector<std::string> tok; tok.reserve(8);
        std::string cur;
        for (char ch : s) {
            if (std::isspace(static_cast<unsigned char>(ch))) { if (!cur.empty()) { tok.emplace_back(cur); cur.clear(); } }
            else cur.push_back(ch);
        }
        if (!cur.empty()) tok.emplace_back(cur);
        if (tok.empty()) continue;
        if (tok[0].size() && tok[0][0] == '#') {
            // Start animation with absolute range syntax: #NAME Rot RlcRef From To
            if (tok.size() == 3) {
                // alt form: #NAME Rot NFrames  (frames to be specified later) → skip for now
                current = &def.anims[tok[0]]; current->enabled = true; current->rotations = std::stoi(tok[1]);
                continue;
            }
            if (tok.size() >= 5) {
                current = &def.anims[tok[0]]; current->enabled = true; current->rotations = std::stoi(tok[1]);
                int rlcRef = std::stoi(tok[2]); int from = std::stoi(tok[3]); int to = std::stoi(tok[4]);
                int dz = (to >= from) ? 1 : -1; int count = (to >= from) ? (to - from + 1) : (from - to + 1);
                current->frames.reserve(current->frames.size() + count);
                const auto it = g_rlc.find(rlcRef);
                int dx = 0, dy = 0; if (it != g_rlc.end()) { dx = it->second.dx; dy = it->second.dy; }
                for (int i = 0, sp = from; i < count; ++i, sp += dz) {
                    FrameRef fr{ rlcRef, sp, dx, dy };
                    current->frames.emplace_back(fr);
                }
                continue;
            }
        }
        if (tok[0] == "USERLC" && tok.size() >= 5) {
            // Extend table (local to this MD) and global, mirroring the original behavior
            int p1 = std::stoi(tok[1]); std::string stem = tok[2]; std::string kind = tok[3]; int dx = std::stoi(tok[4]); int dy = (tok.size() >= 6) ? std::stoi(tok[5]) : 0;
            RLCRefEntry e; e.gpStem = to_upper(stem); e.dx = dx; e.dy = dy; e.isShadow = (to_upper(kind) == "SHADOW");
            g_rlc[p1] = e;
            continue;
        }
        // TODO: support other simple commands if needed
    }
    g_units.emplace(base, std::move(def));
    return true;
}

const UnitDef* get_unit_def(const std::string& name) {
    std::string base = name; for (char& c : base) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    auto it = g_units.find(base);
    if (it == g_units.end()) return nullptr;
    return &it->second;
}

} } // namespace legacy::newmon_compat


