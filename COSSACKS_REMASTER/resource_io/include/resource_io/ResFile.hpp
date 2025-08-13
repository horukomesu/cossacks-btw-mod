#pragma once

#include <string>
#include <vector>

namespace resource_io::resfile {

// Read a text file from data root or archives, return its contents split into lines.
// Lines are returned without trailing CR/LF. Returns false if file not found.
bool read_text_lines(const std::string& path, std::vector<std::string>& outLines);

} // namespace resource_io::resfile

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
