#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace resource_io {

class ResFile {
public:
    static std::vector<uint8_t> read_all(const std::string& path);
};

} // namespace resource_io
