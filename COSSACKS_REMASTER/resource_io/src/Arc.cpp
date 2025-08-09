#include "resource_io/Arc.hpp"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <stdexcept>

namespace resource_io {

#pragma pack(push, 1)
struct TGSCarchHDR {
    uint8_t descriptor[6];
    uint16_t version;
    uint16_t key;
    uint32_t entries;
};

struct TGSCarchFAT {
    uint32_t hash; // unused for now
    char fileName[64];
    uint32_t offset;   // stored as bitwise NOT
    uint32_t size;
    uint32_t reserved; // unused
    uint8_t flags;     // non-zero => encrypted
};
#pragma pack(pop)

static inline uint8_t decrypt_byte(uint8_t v, uint8_t key) {
    // Matches isiDecryptMem: v' = ~(v) ^ key
    return static_cast<uint8_t>((~v) ^ key);
}

static void decrypt_buffer(uint8_t* buffer, size_t size, uint8_t key) {
    for (size_t i = 0; i < size; ++i) buffer[i] = decrypt_byte(buffer[i], key);
}

std::string GSCArchive::to_upper_copy(std::string_view s) {
    std::string out(s);
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return out;
}

void GSCArchive::decrypt_inplace(uint8_t* data, size_t size) {
    // The original uses dbKey = ~(HIBYTE(_CRYPT_KEY_)) where _CRYPT_KEY_ is 0x78CD or 0x4EBA (RU)
    // Use default 0x78CD to match english archives
    const uint16_t cryptKey = 0x78CD;
    const uint8_t key = static_cast<uint8_t>(~(static_cast<uint8_t>(cryptKey >> 8)));
    decrypt_buffer(data, size, key);
}

GSCArchive::GSCArchive(std::string path) : _path(std::move(path)) {}
GSCArchive::~GSCArchive() = default;

bool GSCArchive::is_open() const { return _open; }

std::unique_ptr<GSCArchive> GSCArchive::open(const std::string& filePath) {
    auto arc = std::unique_ptr<GSCArchive>(new GSCArchive(filePath));
    if (!arc->load()) return nullptr;
    return arc;
}

bool GSCArchive::load() {
    std::ifstream in(_path, std::ios::binary);
    if (!in) return false;
    in.seekg(0, std::ios::end);
    const auto size = static_cast<size_t>(in.tellg());
    in.seekg(0, std::ios::beg);
    _fileData.resize(size);
    if (size > 0) in.read(reinterpret_cast<char*>(_fileData.data()), size);
    if (size < sizeof(TGSCarchHDR)) return false;

    const auto* hdr = reinterpret_cast<const TGSCarchHDR*>(_fileData.data());
    const size_t fatOffset = sizeof(TGSCarchHDR);
    const size_t fatSize = static_cast<size_t>(hdr->entries) * sizeof(TGSCarchFAT);
    if (size < fatOffset + fatSize) return false;
    const auto* fat = reinterpret_cast<const TGSCarchFAT*>(_fileData.data() + fatOffset);

    _dataStart = fatOffset + fatSize;

    _entries.reserve(hdr->entries);
    for (uint32_t i = 0; i < hdr->entries; ++i) {
        const TGSCarchFAT& fe = fat[i];
        ArchiveEntry e{};
        e.nameUpper = to_upper_copy(std::string_view(fe.fileName, strnlen(fe.fileName, 64)));
        e.size = fe.size;
        e.rawOffsetInFile = fe.offset;
        e.flags = fe.flags;
        _nameToIndex.emplace(e.nameUpper, _entries.size());
        _entries.emplace_back(std::move(e));
    }

    _open = true;
    return true;
}

bool GSCArchive::read_file(std::string_view fileName, std::vector<uint8_t>& outData) const {
    if (!_open) return false;
    const auto key = to_upper_copy(fileName);
    const auto it = _nameToIndex.find(key);
    if (it == _nameToIndex.end()) return false;
    const ArchiveEntry& e = _entries[it->second];
    const size_t offset = _dataStart + static_cast<size_t>(~e.rawOffsetInFile);
    if (offset + e.size > _fileData.size()) return false;
    outData.assign(_fileData.begin() + offset, _fileData.begin() + offset + e.size);
    if (e.flags != 0 && !outData.empty()) {
        decrypt_inplace(outData.data(), outData.size());
    }
    return true;
}

} // namespace resource_io
