#include "resource_io/ResFile.hpp"
#include "resource_io/ResourceIO.hpp"

#include <string>
#include <vector>

namespace resource_io::resfile {

bool read_text_lines(const std::string& path, std::vector<std::string>& outLines) {
    std::vector<unsigned char> data;
    if (!resource_io::read_file_anywhere(path, data)) return false;
    outLines.clear();
    std::string line;
    for (unsigned char b : data) {
        if (b == '\n' || b == '\r') {
            if (!line.empty()) { outLines.emplace_back(line); line.clear(); }
            continue;
        }
        line.push_back(static_cast<char>(b));
    }
    if (!line.empty()) outLines.emplace_back(line);
    return true;
}

} // namespace resource_io::resfile

#include "resource_io/ResFile.hpp"

#include <fstream>

namespace resource_io {

std::vector<uint8_t> ResFile::read_all(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return {};
    in.seekg(0, std::ios::end);
    const std::streampos size = in.tellg();
    if (size <= 0) return {};
    std::vector<uint8_t> buffer(static_cast<size_t>(size));
    in.seekg(0, std::ios::beg);
    in.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

} // namespace resource_io
