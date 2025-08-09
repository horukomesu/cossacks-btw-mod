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
