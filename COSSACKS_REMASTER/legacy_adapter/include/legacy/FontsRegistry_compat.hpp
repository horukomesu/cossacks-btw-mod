#pragma once

#include <memory>
#include <string>

namespace legacy { namespace ui {

class RLCFont;

// Simple registry to obtain RLCFont by legacy names like "rom10", "vinBig".
// It will resolve to appropriate .RLC resource via ResourceIO/GPS.
class FontsRegistry {
public:
    static FontsRegistry& instance();

    // Get or load font by logical name
    RLCFont* get(const std::string& name);

    // Optional: preload mapping name->resource path (e.g., "rom10"->"rom10.rlc")
    void map_name_to_path(const std::string& name, const std::string& resourcePath);

private:
    FontsRegistry();
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} } // namespace legacy::ui


