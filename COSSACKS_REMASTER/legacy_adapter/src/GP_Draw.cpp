#include "legacy/GP_Draw_compat.hpp"

#include <GLFW/glfw3.h>
#include <unordered_map>

#include "engine_core/EngineCore.hpp"
#include "engine_core/Render2D.hpp"
#include "legacy/Fastdraw_compat.hpp"
#include "legacy/SpriteCache.hpp"
#include "resource_io/ResourceIO.hpp"
#include "resource_io/RLC.hpp"
#include "resource_io/GP.hpp"

namespace legacy::gp {

namespace {
// Map GPID -> resource path (RLC for now). In original, GPID indexes preloaded tables.
std::unordered_map<int, std::string> gpid_to_path;
// Optional per-GPID anchor arrays (dx, lx = width shift), loaded from GPI when available
std::unordered_map<int, std::vector<int8_t>> gpid_to_itdx;
std::unordered_map<int, std::vector<int8_t>> gpid_to_itlx;
// Map normalized resource name -> GPID to dedupe
std::unordered_map<std::string, int> name_to_gpid;
int next_gpid = 0;

static bool draw_by_gpid(int gpid, int sprIndex, int x, int y) {
    auto it = gpid_to_path.find(gpid);
    if (it == gpid_to_path.end()) return false;
    // Apply X shift from GP metadata if available
    auto sh = gpid_to_itdx.find(gpid);
    if (sh != gpid_to_itdx.end() && sprIndex >= 0 && sprIndex < static_cast<int>(sh->second.size())) {
        x -= static_cast<int>(sh->second[static_cast<size_t>(sprIndex)]);
    }
    const std::string& path = it->second;
    return sprite_cache::draw_rlc(path, sprIndex, x, y);
}
static bool draw_by_gpid_pal(int gpid, int sprIndex, int x, int y, int paletteId) {
    auto it = gpid_to_path.find(gpid);
    if (it == gpid_to_path.end()) return false;
    auto sh = gpid_to_itdx.find(gpid);
    if (sh != gpid_to_itdx.end() && sprIndex >= 0 && sprIndex < static_cast<int>(sh->second.size())) {
        x -= static_cast<int>(sh->second[static_cast<size_t>(sprIndex)]);
    }
    const std::string& path = it->second;
    return sprite_cache::draw_rlc_pal(path, sprIndex, x, y, paletteId);
}

static bool get_size_by_gpid(int gpid, int sprIndex, int& w, int& h) {
    auto it = gpid_to_path.find(gpid);
    if (it == gpid_to_path.end()) return false;
    resource_io::rlc::RLCTableData tbl;
    if (!resource_io::rlc::load_rlc(it->second, tbl)) return false;
    return resource_io::rlc::get_subimage_size(tbl, sprIndex, w, h);
}
} // namespace

GPSCompat GPS;

void GPSCompat::ShowGP(int x, int y, int gpid, int sprIndex, uint8_t nation) {
    // Apply national palette mapping when nation != 0
    if (nation != 0) {
        if (draw_by_gpid_pal(gpid, sprIndex, x, y, static_cast<int>(nation % 7) + 1)) return;
    }
    if (!draw_by_gpid(gpid, sprIndex, x, y)) {
        engine_core::render2d::draw_rect(static_cast<float>(x), static_cast<float>(y), 64.0f, 64.0f, 0.8f, 0.6f, 0.2f, 1.0f);
    }
}

void GPSCompat::ShowGPLayers(int x, int y, int gpid, int sprIndex, uint8_t nation, int mask) {
    // Basic layer handling: if mask requests palettized draw, use ShowGPPal; else fallback to ShowGP.
    // Real multi-layer composition will be added later with GP metadata.
    const bool usePal = (mask != 0);
    if (usePal) {
        ShowGPPal(x, y, gpid, sprIndex, nation, nullptr);
    } else {
        ShowGP(x, y, gpid, sprIndex, nation);
    }
}

int GPSCompat::GetGPWidth(int gpid, int sprIndex) {
    int w = 0, h = 0;
    if (get_size_by_gpid(gpid, sprIndex, w, h)) return w;
    return 0;
}

int GPSCompat::GetGPHeight(int gpid, int sprIndex) {
    int w = 0, h = 0;
    if (get_size_by_gpid(gpid, sprIndex, w, h)) return h;
    return 0;
}

int GPSCompat::GetGPShift(int gpid, int sprIndex) {
    auto it = gpid_to_itdx.find(gpid);
    if (it != gpid_to_itdx.end() && sprIndex >= 0 && sprIndex < static_cast<int>(it->second.size())) {
        return static_cast<int>(it->second[static_cast<size_t>(sprIndex)]);
    }
    return 0;
}

void GPSCompat::SetAnchors(int gpid, const std::vector<int8_t>& itdx, const std::vector<int8_t>& itlx) {
    gpid_to_itdx[gpid] = itdx;
    gpid_to_itlx[gpid] = itlx;
}

void GPSCompat::ShowGPPal(int x, int y, int gpid, int sprIndex, uint8_t nation, const uint8_t* indexMap256) {
    // Use provided remap if non-null; otherwise map nation 0.. to pal1..pal7.
    if (indexMap256 != nullptr) {
        // Build a transient pal in cache: integrate explicit map as custom palette id - use slot 0 (identity)
        // For now, route to non-pal path since we lack cache key for custom tables; TODO if needed.
        ShowGP(x, y, gpid, sprIndex, nation);
        return;
    }
    const int palId = static_cast<int>(nation % 7) + 1;
    if (!draw_by_gpid_pal(gpid, sprIndex, x, y, palId)) {
        engine_core::render2d::draw_rect(static_cast<float>(x), static_cast<float>(y), 64.0f, 64.0f, 0.2f, 0.2f, 0.2f, 1.0f);
    }
}

int GPSCompat::LocalGP_Load(const std::string& name) {
    auto it = name_to_gpid.find(name);
    if (it != name_to_gpid.end()) return it->second;

    // Try to resolve typical extensions: .gp/.rlc in archives using provided name stem
    // Normalize and strip optional .GP extension to get base stem
    std::string stem = name;
    // normalize slashes for archive lookup
    for (char& c : stem) if (c == '/') c = '\\';
    auto dotPos = stem.find_last_of('.') ;
    if (dotPos != std::string::npos) {
        std::string ext = stem.substr(dotPos + 1);
        for (char& c : ext) c = static_cast<char>(::toupper(static_cast<unsigned char>(c)));
        if (ext == "GP" || ext == "GPI") {
            stem = stem.substr(0, dotPos);
        }
    }

    // Priority: exact path with .rlc, then search by suffix
    std::string resolved;
    // First attempt: direct name + ".rlc"
    {
        std::vector<unsigned char> tmp;
        if (resource_io::read_file_anywhere(stem + ".rlc", tmp)) {
            resolved = stem + ".rlc";
        }
    }
    if (resolved.empty()) {
        // Fall back to suffix search (case-insensitive)
        std::string suffixCandidate = stem;
        if (!resource_io::try_find_entry_by_suffix(suffixCandidate + ".RLC", resolved)) {
            // Try to find by just the last component (e.g., "CURSOR_02.RLC")
            auto pos = suffixCandidate.find_last_of('\\');
            const std::string last = (pos == std::string::npos) ? suffixCandidate : suffixCandidate.substr(pos + 1);
            resource_io::try_find_entry_by_suffix(last + ".RLC", resolved);
        }
    }
    // If still not resolved, try resolving via .GP metadata and infer .RLC by same stem
    if (resolved.empty()) {
        resource_io::gp::GPFile gpfile;
        if (resource_io::gp::load_gp(stem, gpfile) || resource_io::gp::load_gp(name, gpfile)) {
            // Attempt suffix search for RLC by same base name
            std::string suffixCandidate = stem;
            auto pos = suffixCandidate.find_last_of('\\');
            const std::string last = (pos == std::string::npos) ? suffixCandidate : suffixCandidate.substr(pos + 1);
            std::string rlcPath;
            if (resource_io::try_find_entry_by_suffix(last + ".RLC", rlcPath)) {
                resolved = rlcPath;
            }
        }
    }
    if (resolved.empty()) return -1;

    const int gpid = next_gpid++;
    gpid_to_path.emplace(gpid, resolved);
    name_to_gpid.emplace(name, gpid);
    // Try to load GP metadata for anchors if .gp/.gpi exist with same stem
    resource_io::gp::GPFile gpfile;
    if (resource_io::gp::load_gp(name, gpfile)) {
        std::vector<int8_t> itdx, itlx;
        if (resource_io::gp::load_gpi(name, gpfile.numPictures, itdx, itlx)) {
            SetAnchors(gpid, itdx, itlx);
        }
    }
    return gpid;
}

bool GPSCompat::ResolveGPIDPath(int gpid, std::string& outPath) {
    auto it = gpid_to_path.find(gpid);
    if (it == gpid_to_path.end()) return false;
    outPath = it->second;
    return true;
}

// Legacy free-function shim that some code still calls directly
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
