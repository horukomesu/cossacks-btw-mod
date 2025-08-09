#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

namespace legacy::gfile {

class GFILE {
    std::vector<uint8_t> buffer;
    size_t sizeBytes{0};
    size_t bufPos{0};
    bool realText{false};
    FILE* rf{nullptr};

public:
    GFILE() = default;
    ~GFILE();

    bool open(const char* name, const char* mode);
    void close();

    int readByte();
    int checkByte();

    int gscanf(const char* mask, va_list args);
    int ggetch();
    void gprintf(const char* format, va_list args);

    bool isText() const { return realText; }
};

// C-like API
GFILE* Gopen(char* Name, char* Mode);
int Gscanf(GFILE* F, char* mask, ...);
int Ggetch(GFILE* F);
void Gprintf(GFILE* F, const char *format, ...);
void Gclose(GFILE* F);

} // namespace legacy::gfile
