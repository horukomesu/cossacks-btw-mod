#include "legacy/GP_Draw_compat.hpp"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>

#include "engine_core/EngineCore.hpp"
#include "engine_core/Render2D.hpp"
#include "legacy/Fastdraw_compat.hpp"
#include "legacy/SpriteCache.hpp"
#include "resource_io/ResourceIO.hpp"
#include "resource_io/RLC.hpp"
#include "resource_io/GP.hpp"

namespace legacy::gp {

namespace {
// Map GPID -> resource path (RLC). In original, GPID indexes preloaded tables.
std::unordered_map<int, std::string> gpid_to_path;
// Optional GP file stem (without extension) for metadata (sizes/anchors)
std::unordered_map<int, std::string> gpid_to_gpstem;
// Optional per-GPID anchor arrays (dx, lx), loaded from GPI when available
std::unordered_map<int, std::vector<int8_t>> gpid_to_itdx;
std::unordered_map<int, std::vector<int8_t>> gpid_to_itlx;
// Map normalized resource name -> GPID to dedupe
std::unordered_map<std::string, int> name_to_gpid;
int next_gpid = 0;

static bool draw_by_gpid(int gpid, int sprIndex, int x, int y) {
    // Apply X shift from GP metadata if available
    auto sh = gpid_to_itdx.find(gpid);
    if (sh != gpid_to_itdx.end() && sprIndex >= 0 && sprIndex < static_cast<int>(sh->second.size())) {
        x -= static_cast<int>(sh->second[static_cast<size_t>(sprIndex)]);
    }
    // Prefer GP rendering if a GP stem exists
    auto git = gpid_to_gpstem.find(gpid);
    if (git != gpid_to_gpstem.end()) {
        const bool okgp = sprite_cache::draw_gp(git->second, sprIndex, x, y);
        #ifdef _DEBUG
        if (!okgp) {
            std::cout << "[gp] draw_gp failed for GPID=" << gpid << " frame=" << sprIndex << " stem=" << git->second << "\n";
        }
        #endif
        if (okgp) return true;
    }
    auto it = gpid_to_path.find(gpid);
    if (it == gpid_to_path.end()) return false;
    const std::string& path = it->second;
    const bool ok = sprite_cache::draw_rlc(path, sprIndex, x, y);
    #ifdef _DEBUG
    std::cout << "[gp] draw_rlc GPID=" << gpid << " spr=" << sprIndex
              << " at (" << x << "," << y << ") path=" << path << (ok?" ok":" fail") << "\n";
    #endif
    return ok;
}

static bool draw_by_gpid_pal(int gpid, int sprIndex, int x, int y, int paletteId) {
    auto sh = gpid_to_itdx.find(gpid);
    if (sh != gpid_to_itdx.end() && sprIndex >= 0 && sprIndex < static_cast<int>(sh->second.size())) {
        x -= static_cast<int>(sh->second[static_cast<size_t>(sprIndex)]);
    }
    // Prefer GP
    auto git = gpid_to_gpstem.find(gpid);
    if (git != gpid_to_gpstem.end()) {
        const bool okgp = sprite_cache::draw_gp_pal(git->second, sprIndex, x, y, paletteId);
        #ifdef _DEBUG
        if (!okgp) {
            std::cout << "[gp] draw_gp_pal failed for GPID=" << gpid << " frame=" << sprIndex << " palId=" << paletteId << " stem=" << git->second << "\n";
        }
        #endif
        if (okgp) return true;
    }
    auto it = gpid_to_path.find(gpid);
    if (it == gpid_to_path.end()) return false;
    const std::string& path = it->second;
    const bool ok = sprite_cache::draw_rlc_pal(path, sprIndex, x, y, paletteId);
    #ifdef _DEBUG
    std::cout << "[gp] draw_rlc_pal GPID=" << gpid << " spr=" << sprIndex
              << " palId=" << paletteId << " at (" << x << "," << y << ") path=" << path << (ok?" ok":" fail") << "\n";
    #endif
    return ok;
}

static bool get_size_by_gpid(int gpid, int sprIndex, int& w, int& h) {
    // Prefer GP bounds if GP stem exists
    auto itgp = gpid_to_gpstem.find(gpid);
    if (itgp != gpid_to_gpstem.end()) {
        resource_io::gp::GPFile gp;
        if (resource_io::gp::load_gp(itgp->second, gp)) {
            int minx = 0, miny = 0, maxx = 0, maxy = 0;
            if (resource_io::gp::compute_frame_bounds(gp, sprIndex, minx, miny, maxx, maxy)) {
                w = std::max(1, maxx - minx);
                h = std::max(1, maxy - miny);
                return true;
            }
        }
    }
    // Fallback to RLC size
    auto it = gpid_to_path.find(gpid);
    if (it != gpid_to_path.end()) {
        resource_io::rlc::RLCTableData tbl;
        if (resource_io::rlc::load_rlc(it->second, tbl)) {
            return resource_io::rlc::get_subimage_size(tbl, sprIndex, w, h);
        }
    }
    return false;
}
} // namespace

GPSCompat GPS;

void GPSCompat::ShowGP(int x, int y, int gpid, int sprIndex, uint8_t nation) {
    // For UI we want base palette even when nation == 0 (so that indices map through agew_1.pal)
    if (draw_by_gpid_pal(gpid, sprIndex, x, y, /*paletteId*/ 0)) return;
    if (!draw_by_gpid(gpid, sprIndex, x, y)) {
        // Visual aid: draw a placeholder rect when resource missing
        engine_core::render2d::draw_rect(static_cast<float>(x), static_cast<float>(y), 64.0f, 64.0f, 0.8f, 0.6f, 0.2f, 1.0f);
    }
}

void GPSCompat::ShowGPLayers(int x, int y, int gpid, int sprIndex, uint8_t nation, int mask) {
    const bool usePal = (mask != 0);
    if (usePal) {
        ShowGPPal(x, y, gpid, sprIndex, nation, nullptr);
    } else {
        ShowGP(x, y, gpid, sprIndex, nation);
    }
}

int GPSCompat::GetGPWidth(int gpid, int sprIndex) {
    int w = 0, h = 0;
    if (get_size_by_gpid(gpid, sprIndex, w, h)) {
        #ifdef _DEBUG
        std::cout << "[gp] GetGPWidth GPID=" << gpid << " spr=" << sprIndex << " -> " << w << "\n";
        #endif
        return w;
    }
    return 0;
}

int GPSCompat::GetGPHeight(int gpid, int sprIndex) {
    int w = 0, h = 0;
    if (get_size_by_gpid(gpid, sprIndex, w, h)) {
        #ifdef _DEBUG
        std::cout << "[gp] GetGPHeight GPID=" << gpid << " spr=" << sprIndex << " -> " << h << "\n";
        #endif
        return h;
    }
    return 0;
}

int GPSCompat::GetGPShift(int gpid, int sprIndex) {
    auto it = gpid_to_itdx.find(gpid);
    if (it != gpid_to_itdx.end() && sprIndex >= 0 && sprIndex < static_cast<int>(it->second.size())) {
        int shift = static_cast<int>(it->second[static_cast<size_t>(sprIndex)]);
        #ifdef _DEBUG
        std::cout << "[gp] GetGPShift GPID=" << gpid << " spr=" << sprIndex << " -> " << shift << "\n";
        #endif
        return shift;
    }
    return 0;
}

void GPSCompat::SetAnchors(int gpid, const std::vector<int8_t>& itdx, const std::vector<int8_t>& itlx) {
    gpid_to_itdx[gpid] = itdx;
    gpid_to_itlx[gpid] = itlx;
}

void GPSCompat::ShowGPPal(int x, int y, int gpid, int sprIndex, uint8_t nation, const uint8_t* indexMap256) {
    if (indexMap256 != nullptr) {
        // Not yet supporting explicit custom tables in cache key; fall back to nation mapping
        ShowGP(x, y, gpid, sprIndex, nation);
        return;
    }
    const int palId = static_cast<int>(nation % 7) + 1;
    if (!draw_by_gpid_pal(gpid, sprIndex, x, y, palId)) {
        engine_core::render2d::draw_rect(static_cast<float>(x), static_cast<float>(y), 64.0f, 64.0f, 0.2f, 0.2f, 0.2f, 1.0f);
    }
}

int GPSCompat::LocalGP_Load(const std::string& name) {
    auto itFound = name_to_gpid.find(name);
    if (itFound != name_to_gpid.end()) return itFound->second;

    // Normalize path separators
    std::string stem = name;
    for (char& c : stem) if (c == '/') c = '\\';
    // Strip optional .GP/.GPI extension
    auto dotPos = stem.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string ext = stem.substr(dotPos + 1);
        for (char& c : ext) c = static_cast<char>(::toupper(static_cast<unsigned char>(c)));
        if (ext == "GP" || ext == "GPI") stem = stem.substr(0, dotPos);
    }

    std::string resolved;
    bool gp_ok = false;
    resource_io::gp::GPFile gpfile_meta;
    // Try direct GP first (original: .gp is primary)
    if (resource_io::gp::load_gp(stem, gpfile_meta)) {
        gp_ok = true;
    }
    // Then try direct RLC by stem (archives store uppercase names)
    {
        std::vector<unsigned char> tmp;
        if (resource_io::read_file_anywhere(stem + ".rlc", tmp)) {
            resolved = stem + ".rlc";
        }
    }
    // Special case: original cursor packs Cursor_00/01/02 share a common RLC named CURSOR_Y.RLC
    if (resolved.empty()) {
        auto pos = stem.find_last_of('\\');
        const std::string last = (pos == std::string::npos) ? stem : stem.substr(pos + 1);
        std::string lastUp = last;
        for (char& c : lastUp) c = static_cast<char>(::toupper(static_cast<unsigned char>(c)));
        if (lastUp == "CURSOR_00" || lastUp == "CURSOR_01" || lastUp == "CURSOR_02") {
            std::string r;
            if (resource_io::try_find_entry_by_suffix("CURSOR_Y.RLC", r)) {
                resolved = r;
            }
        }
    }
    if (resolved.empty()) {
        // Search in archives by suffix (case-insensitive, uppercase)
        std::string r;
        if (resource_io::try_find_entry_by_suffix(stem + ".RLC", r)) {
            resolved = r;
        } else {
            // Try last component only (e.g., MAIN_MENU.RLC)
            auto pos = stem.find_last_of('\\');
            const std::string last = (pos == std::string::npos) ? stem : stem.substr(pos + 1);
            if (resource_io::try_find_entry_by_suffix(last + ".RLC", r)) resolved = r;
        }
    }
    // If neither GP nor RLC found, fail
    if (!gp_ok && resolved.empty()) {
        #ifdef _DEBUG
        std::cout << "[gp] LocalGP_Load failed: name=" << name << "\n";
        #endif
        return -1;
    }

    const int gpid = next_gpid++;
    name_to_gpid.emplace(name, gpid);
    if (!resolved.empty()) {
        gpid_to_path.emplace(gpid, resolved);
    }
    if (gp_ok) {
        gpid_to_gpstem.emplace(gpid, stem);
        std::vector<int8_t> itdx, itlx;
        if (resource_io::gp::load_gpi(stem, gpfile_meta.numPictures, itdx, itlx)) {
            SetAnchors(gpid, itdx, itlx);
        }
        #ifdef _DEBUG
        std::cout << "[gp] LocalGP_Load: name=" << name << " -> GPID=" << gpid << " gp=" << stem;
        if (!resolved.empty()) std::cout << " rlc=" << resolved;
        std::cout << "\n";
        #else
        (void)resolved;
        #endif
    } else {
        #ifdef _DEBUG
        std::cout << "[gp] LocalGP_Load: name=" << name << " -> GPID=" << gpid << " path=" << resolved << "\n";
        #endif
    }
    return gpid;
}

bool GPSCompat::ResolveGPIDPath(int gpid, std::string& outPath) {
    auto it = gpid_to_path.find(gpid);
    if (it == gpid_to_path.end()) return false;
    outPath = it->second;
    return true;
}

// Legacy free-function shims
void ShowGP(int x, int y, int fileIndex, int sprIndex, uint8_t nation) {
    GPS.ShowGP(x, y, fileIndex, sprIndex, nation);
}

void FlipPages() {
    if (GLFWwindow* w = engine_core::get_active_window()) {
        glfwSwapBuffers(w);
    }
}

void SetRLCWindow(int x, int y, int lx, int ly, int slx) {
    legacy::fastdraw::SetRLCWindow(x, y, lx, ly, slx);
}

void CBar(int x, int y, int Lx, int Ly, uint8_t c) {
    legacy::fastdraw::CBar(x, y, Lx, Ly, c);
}

void CopyToScreen(int x, int y, int Lx, int Ly) {
    legacy::fastdraw::CopyToScreen(x, y, Lx, Ly);
}

} // namespace legacy::gp

