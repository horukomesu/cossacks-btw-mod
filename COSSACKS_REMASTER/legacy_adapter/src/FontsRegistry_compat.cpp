#include "../include/legacy/FontsRegistry_compat.hpp"

#include "legacy/RLCFont_compat.hpp"
#include "resource_io/ResourceIO.hpp"

#include <unordered_map>

namespace legacy { namespace ui {

struct FontsRegistry::Impl {
    std::unordered_map<std::string, std::unique_ptr<RLCFont>> nameToFont;
    std::unordered_map<std::string, std::string> nameToPath;
};

FontsRegistry& FontsRegistry::instance() {
    static FontsRegistry inst;
    return inst;
}

FontsRegistry::FontsRegistry() : pimpl(std::make_unique<Impl>()) {}

void FontsRegistry::map_name_to_path(const std::string& name, const std::string& resourcePath) {
    pimpl->nameToPath[name] = resourcePath;
}

RLCFont* FontsRegistry::get(const std::string& name) {
    auto it = pimpl->nameToFont.find(name);
    if (it != pimpl->nameToFont.end()) return it->second.get();

    // Resolve path by known map or try common patterns
    std::string path;
    auto itp = pimpl->nameToPath.find(name);
    if (itp != pimpl->nameToPath.end()) {
        path = itp->second;
    } else {
        // Try exact name as path, then with .RLC and search by suffix
        path = name;
        std::string found;
        std::vector<unsigned char> dummy;
        if (!resource_io::read_file_anywhere(path, dummy)) {
            if (resource_io::try_find_entry_by_suffix(name + ".RLC", found)) path = found;
            else if (resource_io::try_find_entry_by_suffix(name + ".rlc", found)) path = found;
            else if (resource_io::try_find_entry_by_suffix(name, found)) path = found;
        }
    }

    auto font = std::make_unique<RLCFont>(path.c_str());
    // Default base char = 32; tweak common colors per logical name
    if (name == "rom10" || name == "vinBig") {
        font->SetWhiteColor();
    }
    RLCFont* ret = font.get();
    pimpl->nameToFont.emplace(name, std::move(font));
    return ret;
}

} } // namespace legacy::ui


