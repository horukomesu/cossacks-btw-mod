from __future__ import annotations

import struct
from dataclasses import dataclass
from typing import List, Tuple


_I32 = struct.Struct('<i')
_RLC_HDR = struct.Struct('<hh')  # SizeX, SizeY (int16)


@dataclass
class RlcFrameInfo:
    index: int
    width: int
    height: int


@dataclass
class RlcSummary:
    nimages: int
    frames: List[RlcFrameInfo]


def _read_table_header(blob: bytes) -> Tuple[int, List[int]]:
    if len(blob) < 12:
        raise ValueError('RLC file too small')
    # xRLCTable: int Size; int sign; int SCount; int OfsTable[...]
    size, sign, scount = struct.unpack_from('<iii', blob, 0)
    if scount < 0:
        scount &= 0xFFFF
    # Offsets follow immediately after header
    offs: List[int] = []
    base = 12
    for i in range(scount):
        pos = base + i * 4
        if pos + 4 > len(blob):
            break
        off = _I32.unpack_from(blob, pos)[0]
        offs.append(off)
    if len(offs) != scount:
        raise ValueError('RLC offsets truncated')
    return scount, offs


def parse_rlc_summary(blob: bytes) -> RlcSummary:
    n, offs = _read_table_header(blob)
    frames: List[RlcFrameInfo] = []
    for i, off in enumerate(offs):
        if off + _RLC_HDR.size > len(blob):
            break
        sx, sy = _RLC_HDR.unpack_from(blob, off)
        w = int(sx & 0xFFFF)
        h = int(sy & 0xFFFF)
        frames.append(RlcFrameInfo(i, w, h))
    return RlcSummary(nimages=n, frames=frames)


def decode_rlc_indexed(blob: bytes, index: int) -> Tuple[bytes, int, int]:
    n, offs = _read_table_header(blob)
    if index < 0 or index >= n:
        raise IndexError('RLC index out of range')
    off = offs[index]
    if off + _RLC_HDR.size > len(blob):
        raise ValueError('RLC subimage header out of range')
    sx, sy = _RLC_HDR.unpack_from(blob, off)
    width = int(sx & 0xFFFF)
    height = int(sy & 0xFFFF)
    ptr = off + _RLC_HDR.size
    out = bytearray(width * height)
    for y in range(height):
        if ptr >= len(blob):
            break
        segs = blob[ptr]
        ptr += 1
        x = 0
        for _ in range(segs):
            if ptr + 2 > len(blob):
                break
            skip = blob[ptr]
            run = blob[ptr + 1]
            ptr += 2
            x += skip
            # copy run pixels
            for i in range(run):
                if ptr >= len(blob):
                    break
                if 0 <= x + i < width:
                    out[y * width + (x + i)] = blob[ptr]
                ptr += 1
            x += run
    return bytes(out), width, height


def decode_rlc_rgba(blob: bytes, index: int, palette_rgb_768: bytes | None = None) -> Tuple[bytes, int, int]:
    idx, w, h = decode_rlc_indexed(blob, index)
    rgba = bytearray(w * h * 4)
    if palette_rgb_768 and len(palette_rgb_768) >= 768:
        for i, v in enumerate(idx):
            o = i * 4
            if v == 0:
                rgba[o:o+4] = b"\x00\x00\x00\x00"
            else:
                p = v * 3
                r = palette_rgb_768[p]
                g = palette_rgb_768[p + 1]
                b = palette_rgb_768[p + 2]
                rgba[o:o+4] = bytes((r, g, b, 255))
    else:
        for i, v in enumerate(idx):
            o = i * 4
            if v == 0:
                rgba[o:o+4] = b"\x00\x00\x00\x00"
            else:
                rgba[o:o+4] = bytes((v, v, v, 255))
    return bytes(rgba), w, h


