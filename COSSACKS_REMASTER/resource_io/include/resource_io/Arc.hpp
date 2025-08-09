#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace resource_io {

struct ArchiveEntry {
    std::string nameUpper;
    uint32_t size = 0;
    uint32_t rawOffsetInFile = 0; // stored as bitwise NOT in FAT; we compute offset = dataStart + ~rawOffsetInFile
    uint8_t flags = 0;            // non-zero means encrypted, requires decryption
};

class GSCArchive {
public:
    static std::unique_ptr<GSCArchive> open(const std::string& filePath);

    ~GSCArchive();
    bool is_open() const;

    const std::vector<ArchiveEntry>& list() const { return _entries; }

    // Reads a file by case-insensitive name; returns false if not found
    bool read_file(std::string_view fileName, std::vector<uint8_t>& outData) const;

    const std::string& path() const { return _path; }

private:
    explicit GSCArchive(std::string path);
    bool load();

    // Helpers
    static std::string to_upper_copy(std::string_view s);
    static void decrypt_inplace(uint8_t* data, size_t size);

    std::string _path;
    std::vector<uint8_t> _fileData; // entire archive file
    size_t _dataStart = 0;           // offset where bulk data begins
    bool _open = false;
    std::vector<ArchiveEntry> _entries;
    std::unordered_map<std::string, size_t> _nameToIndex; // upper name -> index in _entries
};

} // namespace resource_io
