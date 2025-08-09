from __future__ import annotations

import struct
from typing import List, Tuple

from .gp_parser import _GPHDR_FIXED, _LGPH_COUNT, _U32, _GP_PICT_HDR


def std_unpack(src: bytes, dest_len: int, voc: bytes) -> bytes:
    out = bytearray(dest_len)
    si = 0
    di = 0
    slen = len(src)
    vlen = len(voc)
    while di < dest_len and si < slen:
        flags = src[si]
        si += 1
        for _ in range(8):
            if flags & 0x80:
                if si + 2 > slen:
                    return bytes(out)
                ctrl = src[si] | (src[si + 1] << 8)
                si += 2
                length = ((ctrl >> 12) & 0xF) + 3
                ofs = ctrl & 0x0FFF
                if ofs >= vlen:
                    ofs %= max(1, vlen)
                # copy from voc
                chunk = voc[ofs : ofs + length]
                out[di : di + len(chunk)] = chunk
                di += len(chunk)
            else:
                if si >= slen or di >= dest_len:
                    return bytes(out)
                out[di] = src[si]
                si += 1
                di += 1
            flags = (flags << 1) & 0xFF
            if di >= dest_len:
                break
    return bytes(out)


def lz_unpack(src: bytes, dest_len: int) -> bytes:
    out = bytearray(dest_len)
    si = 0
    di = 0
    slen = len(src)
    while di < dest_len and si < slen:
        flags = src[si]
        si += 1
        for _ in range(8):
            if flags & 1:
                if si + 2 > slen:
                    return bytes(out)
                ctrl = src[si] | (src[si + 1] << 8)
                si += 2
                ofs = ctrl & 0x1FFF
                length = (ctrl >> 13) + 3
                src_index = di - ofs - 1
                for _k in range(length):
                    if 0 <= src_index < di:
                        out[di] = out[src_index]
                    else:
                        out[di] = 0
                    di += 1
                    src_index += 1
                    if di >= dest_len:
                        break
            else:
                if si >= slen or di >= dest_len:
                    return bytes(out)
                out[di] = src[si]
                si += 1
                di += 1
            flags >>= 1
            if di >= dest_len:
                break
    return bytes(out)


def nat_unpack(src: bytes, dest_len: int) -> bytes:
    out = bytearray(dest_len)
    si = 0
    di = 0
    need = dest_len // 4
    for _ in range(need):
        if si >= len(src):
            break
        a = src[si]
        si += 1
        # bytes: low2, bits[3:2], bits[5:4], bits[7:6]
        b0 = (a & 0x03)
        b1 = (a >> 2) & 0x03
        b2 = (a >> 4) & 0x03
        b3 = (a >> 6) & 0x03
        out[di : di + 4] = bytes((b0, b1, b2, b3))
        di += 4
    return bytes(out)


def grey_unpack(src: bytes, dest_len: int) -> bytes:
    out = bytearray(dest_len)
    si = 0
    di = 0
    need = dest_len // 2
    for _ in range(need):
        if si >= len(src):
            break
        a = src[si]
        si += 1
        b_low = (a & 0x0F) << 1
        b_high = ((a >> 4) & 0x0F) >> 3
        # follow assembly more closely: high becomes (high >> 3), which is either 0 or 1
        out[di] = b_low
        out[di + 1] = b_high
        di += 2
    return bytes(out)


def _compute_frame_bounds(blob: bytes, base: int, frame_off: int) -> Tuple[int, int, int, int]:
    # returns (minx, miny, maxx, maxy) in frame space
    pos = base + frame_off
    minx = 1_000_000
    miny = 1_000_000
    maxx = -1_000_000
    maxy = -1_000_000
    while True:
        if pos + _GP_PICT_HDR.size > len(blob):
            break
        nextp, dx, dy, lx, ly, _pack, opt, cdata, nlines = _GP_PICT_HDR.unpack_from(blob, pos)
        minx = min(minx, dx)
        miny = min(miny, dy)
        maxx = max(maxx, dx + lx)
        # Use Ly for bounding box like engine GetLy()
        maxy = max(maxy, dy + ly)
        if nextp == -1:
            break
        pos = base + frame_off + nextp
    if minx > maxx or miny > maxy:
        return 0, 0, 0, 0
    return minx, miny, maxx, maxy


def render_gp_frame(
    blob: bytes,
    frame_index: int,
    palette: bytes | None = None,
    overlay: str | None = None,
    natpal: bytes | None = None,
) -> Tuple[bytes, int, int]:
    # Returns (RGBA bytes, width, height)
    # Parse header
    if len(blob) < _GPHDR_FIXED.size + _LGPH_COUNT * 4:
        raise ValueError('GP blob too small')
    sign, npic, _res, voc_off, voc_len = _GPHDR_FIXED.unpack_from(blob, 0)
    if frame_index < 0 or frame_index >= npic:
        raise IndexError('frame index out of range')
    base = 0
    lgph_base = _GPHDR_FIXED.size
    frame_off = _U32.unpack_from(blob, lgph_base + frame_index * 4)[0]
    minx, miny, maxx, maxy = _compute_frame_bounds(blob, base, frame_off)
    width = max(1, maxx - minx)
    height = max(1, maxy - miny)
    # 8-bit index canvas (0=transparent)
    canvas = [[0] * width for _ in range(height)]
    voc = blob[voc_off : voc_off + voc_len] if voc_len > 0 else b''

    pos = base + frame_off
    while True:
        nextp, dx, dy, lx, ly, _pack, opt, cdata, nlines = _GP_PICT_HDR.unpack_from(blob, pos)
        unpack_len = (cdata >> 14) & 0x3FFFF
        cdoffs = cdata & 0x3FFF
        if opt & 64:
            cdoffs += 16384
        if opt & 128:
            cdoffs += 32768
        opt_code = opt & 63
        if opt_code == 43:
            unpack_len += 262144
        elif opt_code == 44:
            unpack_len += 524288

        src_ptr = pos + cdoffs
        src = blob[src_ptr : src_ptr + max(0, unpack_len)]
        if opt_code == 0:
            cdata_bytes = std_unpack(src, unpack_len, voc)
        elif opt_code in (42, 43, 44):
            cdata_bytes = lz_unpack(src, unpack_len)
        elif opt_code == 41:
            cdata_bytes = src[:unpack_len]
        elif opt_code == 1:
            cdata_bytes = nat_unpack(src, unpack_len)
        elif opt_code == 38:
            cdata_bytes = grey_unpack(src, unpack_len)
        else:
            # best-effort fallback
            cdata_bytes = src[:unpack_len]

        # Decode scanlines using line descriptors stream at pos+23
        line_ptr = pos + 23
        cdpos = 0
        for line in range(nlines):
            if line_ptr >= len(blob):
                break
            head = blob[line_ptr]
            line_ptr += 1
            x = 0
            if head & 0x80:  # complex line
                segs = head & 31
                # ext adds bit 4 to run length (OR), equivalent to +16 for lengths 0..15
                ext = 16 if (head & 32) else 0
                for _ in range(segs):
                    if line_ptr >= len(blob):
                        break
                    b = blob[line_ptr]
                    line_ptr += 1
                    xdelta = b & 0x0F
                    ln = (b >> 4) | ext
                    gx = x + xdelta
                    gy = (dy - miny) + line
                    if 0 <= gy < height:
                        for i in range(ln):
                            if cdpos >= len(cdata_bytes):
                                break
                            px = gx + i + (dx - minx)
                            if 0 <= px < width:
                                v = cdata_bytes[cdpos]
                                if v:
                                    canvas[gy][px] = v
                            cdpos += 1
                    else:
                        cdpos += ln
                    x = gx + ln
            else:  # simple line
                segs = head
                for _ in range(segs):
                    if line_ptr + 1 >= len(blob):
                        break
                    xdelta = blob[line_ptr]
                    ln = blob[line_ptr + 1]
                    line_ptr += 2
                    gx = x + xdelta
                    gy = (dy - miny) + line
                    if 0 <= gy < height:
                        for i in range(ln):
                            if cdpos >= len(cdata_bytes):
                                break
                            px = gx + i + (dx - minx)
                            if 0 <= px < width:
                                v = cdata_bytes[cdpos]
                                if v:
                                    canvas[gy][px] = v
                            cdpos += 1
                    else:
                        cdpos += ln
                    x = gx + ln

        if nextp == -1:
            break
        pos = base + frame_off + nextp

    # Map 8-bit indices to RGBA using provided palette (768 bytes R,G,B*256) if available
    rgba = bytearray(width * height * 4)
    di = 0
    for y in range(height):
        row = canvas[y]
        for v in row:
            if v == 0:
                rgba[di : di + 4] = b"\x00\x00\x00\x00"
            else:
                if palette and len(palette) >= 768:
                    off = v * 3
                    r = palette[off]
                    g = palette[off + 1]
                    b = palette[off + 2]
                    rgba[di : di + 4] = bytes((r, g, b, 255))
                else:
                    c = v
                    rgba[di : di + 4] = bytes((c, c, c, 255))
            di += 4

    # Placeholder for overlay modes: apply nation palette, alpha, trans tables if provided later
    if overlay == 'natpal' and natpal:
        pass
    return bytes(rgba), width, height


