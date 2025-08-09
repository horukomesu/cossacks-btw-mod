from __future__ import annotations

import struct
from dataclasses import dataclass
from typing import List, Tuple


# Structures per src/Main executable/GP_Draw.h (packed, little-endian)
_GPHDR_FIXED = struct.Struct('<ihhih')  # Sign(int), NPictures(short), Reserved(short), VocOffset(int), VocLength(short)
_LGPH_COUNT = 256
_U32 = struct.Struct('<I')

# GP_Header: int NextPict; short dx; short dy; short Lx; short Ly; uint32 Pack; uint8 Options; uint32 CData; short NLines
_GP_PICT_HDR = struct.Struct('<ihhhhIBIh')


@dataclass
class GpFrameInfo:
    index: int
    dx: int
    dy: int
    lx: int
    ly: int


@dataclass
class GpSummary:
    sign: int
    npictures: int
    voc_offset: int
    voc_length: int
    frames: List[GpFrameInfo]


def parse_gp_summary(blob: bytes) -> GpSummary:
    if len(blob) < _GPHDR_FIXED.size + _LGPH_COUNT * 4:
        raise ValueError('GP file too small')
    sign, npic, _res, voc_off, voc_len = _GPHDR_FIXED.unpack_from(blob, 0)
    # Read 256 relative offsets
    lgph_base = _GPHDR_FIXED.size
    frames: List[GpFrameInfo] = []
    for i in range(npic):
        rel = _U32.unpack_from(blob, lgph_base + i * 4)[0]
        if rel == 0:
            continue
        pos = rel  # relative to file start of GP header (since we unpack from start)
        if pos + _GP_PICT_HDR.size > len(blob):
            break
        nextp, dx, dy, lx, ly, _pack, _opt, _cdata, _nlines = _GP_PICT_HDR.unpack_from(blob, pos)
        frames.append(GpFrameInfo(i, dx, dy, lx, ly))
    return GpSummary(sign=sign, npictures=npic, voc_offset=voc_off, voc_length=voc_len, frames=frames)


