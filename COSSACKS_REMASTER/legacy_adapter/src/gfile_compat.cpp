#include "legacy/gfile_compat.hpp"

#include "resource_io/ResourceIO.hpp"

#include <cassert>
#include <cctype>
#include <cstring>

namespace legacy::gfile {

GFILE::~GFILE() { close(); }

bool GFILE::open(const char* name, const char* mode) {
    if (!name || !mode) return false;
    if (mode[0] == 'w') {
        rf = std::fopen(name, mode);
        realText = (rf != nullptr);
        return realText;
    }
    std::vector<unsigned char> data;
    if (!resource_io::read_file_anywhere(name, data)) return false;
    buffer.assign(data.begin(), data.end());
    sizeBytes = buffer.size();
    bufPos = 0;
    realText = false;
    return true;
}

void GFILE::close() {
    if (rf) {
        std::fclose(rf);
        rf = nullptr;
    }
    buffer.clear();
    sizeBytes = 0;
    bufPos = 0;
    realText = false;
}

int GFILE::readByte() {
    if (realText) return EOF;
    if (bufPos < sizeBytes) return buffer[bufPos++];
    return -1;
}

int GFILE::checkByte() {
    if (realText) return EOF;
    if (bufPos < sizeBytes) return buffer[bufPos];
    return -1;
}

int GFILE::gscanf(const char* Mask, va_list args) {
    int spos = 0;
    char c = 0;
    char* v_char = nullptr;
    int* v_int = nullptr;
    int nargret = 0;
    do {
        c = Mask[spos];
        if (c == '%') {
            spos++;
            c = Mask[spos];
            spos++;
            switch (c) {
            case 's':
            case 'S': {
                v_char = va_arg(args, char*);
                int cc = 0;
                int NL = 0;
                bool exitLoop = false;
                do {
                    cc = checkByte();
                    if (!NL) {
                        if (!(cc == 0x0D || cc == 0x0A || cc == ' ' || cc == 9 || cc == -1)) {
                            v_char[NL] = static_cast<char>(cc);
                            NL++;
                        }
                        if (cc == -1) exitLoop = true;
                        readByte();
                    } else {
                        if (cc == 0x0D || cc == 0x0A || cc == ' ' || cc == 9 || cc == -1) {
                            exitLoop = true;
                        } else {
                            v_char[NL] = static_cast<char>(cc);
                            NL++;
                            readByte();
                        }
                    }
                } while (!exitLoop);
                v_char[NL] = 0;
                if (NL) nargret++; else return nargret;
            } break;
            case 'd':
            case 'D':
            case 'g': {
                int cc = 0;
                int NL = 0;
                char vcr[32]{};
                bool exitLoop = false;
                do {
                    cc = checkByte();
                    if (!NL) {
                        if (!(cc == 0x0D || cc == 0x0A || cc == ' ' || cc == 9)) {
                            if ((cc >= '0' && cc <= '9') || cc == '.' || cc == '-') {
                                vcr[NL++] = static_cast<char>(cc);
                            } else exitLoop = true;
                        }
                        if (cc == -1) exitLoop = true;
                        readByte();
                    } else {
                        if (NL < 20) {
                            if ((cc < '0' || cc > '9') && cc != '.' && cc != '-') exitLoop = true;
                            else { vcr[NL++] = static_cast<char>(cc); readByte(); }
                        } else exitLoop = true;
                    }
                } while (!exitLoop);
                vcr[NL] = 0;
                if (vcr[0]) {
                    if (c == 'g') {
                        float* darg = va_arg(args, float*);
                        int z = std::sscanf(vcr, "%g", darg);
                        if (z != 1) return nargret; else nargret++;
                    } else {
                        v_int = va_arg(args, int*);
                        int z = std::sscanf(vcr, "%d", v_int);
                        if (z != 1) return nargret; else nargret++;
                    }
                } else return nargret;
            } break;
            case 'l': {
                c = Mask[spos];
                (void)c; // expecting 'c'
                v_char = va_arg(args, char*);
                int cc = readByte();
                if (cc != -1) { v_char[0] = static_cast<char>(cc); nargret++; }
                else return nargret;
            } break;
            default:
                assert(false && "Unsupported format");
            }
        } else {
            spos++;
        }
    } while (c != '\0');
    return nargret;
}

int GFILE::ggetch() {
    int cc = readByte();
    if (cc == 0x0D) cc = readByte();
    return cc;
}

void GFILE::gprintf(const char* format, va_list args) {
    if (rf) vfprintf(rf, format, args);
}

// C-like API wrappers
GFILE* Gopen(char* Name, char* Mode) {
    auto* F = new GFILE();
    if (!F->open(Name, Mode)) { delete F; return nullptr; }
    return F;
}

int Gscanf(GFILE* F, char* mask, ...) {
    va_list args; va_start(args, mask);
    int z = F->gscanf(mask, args);
    va_end(args);
    return z;
}

int Ggetch(GFILE* F) { return F->ggetch(); }

void Gprintf(GFILE* F, const char *format, ...) {
    va_list args; va_start(args, format);
    F->gprintf(format, args);
    va_end(args);
}

void Gclose(GFILE* F) { if (!F) return; F->close(); delete F; }

} // namespace legacy::gfile
