from __future__ import annotations

import io
import os
import struct
from dataclasses import dataclass
from typing import Iterable, List, Optional, Tuple


# Based on src/Main executable/Arc/GSCtypes.h and GSCarch.cpp
# TGSCarchHDR
#  BYTE   m_Descriptor[6]
#  WORD   m_Version
#  WORD   m_Key
#  DWORD  m_Entries
# TGSCarchFAT
#  DWORD  m_Hash
#  BYTE   m_FileName[64]
#  DWORD  m_Offset   // stored bitwise NOT (~offset)
#  DWORD  m_Size
#  DWORD  m_Reserved
#  BYTE   m_Flags    // if non-zero, decrypt with key=~HIBYTE(_CRYPT_KEY_)


GSC_HEADER_STRUCT = struct.Struct("<6sHHI")
GSC_FAT_STRUCT = struct.Struct("<I64sIIIB")


def calc_hash(name_upper_64: bytes) -> int:
    # Equivalent to isiCalcHash (sum of 16 byte-swapped dwords)
    if len(name_upper_64) != 64:
        raise ValueError("name_upper_64 must be 64 bytes")
    total = 0
    for i in range(0, 64, 4):
        d2 = int.from_bytes(name_upper_64[i : i + 4], "big")
        total = (total + d2) & 0xFFFFFFFF
    return total


def isi_decrypt_mem(buf: bytearray, key_byte: int) -> None:
    # From isiDecryptMem: not byte, then xor with key
    kb = key_byte & 0xFF
    for i in range(len(buf)):
        v = (~buf[i]) & 0xFF
        buf[i] = v ^ kb


def isi_encrypt_mem(buf: bytearray, key_byte: int) -> None:
    # Inverse of decrypt (isiEncryptMem uses not key, xor, not)
    kb = (~key_byte) & 0xFF
    for i in range(len(buf)):
        v = buf[i] ^ kb
        buf[i] = (~v) & 0xFF


def normalize_name(name: str) -> bytes:
    up = name.replace("/", "\\").upper().encode("ascii", "ignore")
    if len(up) > 63:
        raise ValueError("File name too long for GSC (max 63 chars)")
    return up.ljust(64, b"\x00")


@dataclass
class GscEntry:
    name: str
    size: int
    offset: int  # absolute data offset in the file
    flags: int
    reserved: int
    hash: int


class GscArchive:
    def __init__(self, path: str):
        self.path = path
        self._fp: Optional[io.BufferedReader] = None
        self.version: int = 0
        self.key_word: int = 0x78CD
        self.key_byte: int = (~((self.key_word >> 8) & 0xFF)) & 0xFF  # default EN
        self.entries: List[GscEntry] = []
        self._data_start: int = 0

    def open(self) -> None:
        fp = open(self.path, "rb")
        self._fp = fp
        hdr = fp.read(GSC_HEADER_STRUCT.size)
        if len(hdr) != GSC_HEADER_STRUCT.size:
            raise IOError("Invalid GSC header")
        desc, version, key_word, entries = GSC_HEADER_STRUCT.unpack(hdr)
        self.version = version
        # Keep header key_word to allow switching RU key if needed on write
        self.key_word = key_word
        self.key_byte = (~((self.key_word >> 8) & 0xFF)) & 0xFF
        self.entries.clear()
        fats: List[GscEntry] = []
        for _ in range(entries):
            raw = fp.read(GSC_FAT_STRUCT.size)
            if len(raw) != GSC_FAT_STRUCT.size:
                raise IOError("Corrupted GSC FAT")
            h, nm, off, sz, resv, flg = GSC_FAT_STRUCT.unpack(raw)
            nm = nm.split(b"\x00", 1)[0].decode("ascii", "ignore")
            # Offset is stored inverted (~offset)
            true_off = (~off) & 0xFFFFFFFF
            fats.append(
                GscEntry(
                    name=nm,
                    size=sz,
                    offset=true_off,  # absolute data offset from file start of data section
                    flags=flg & 0xFF,
                    reserved=resv,
                    hash=h,
                )
            )
        self._data_start = fp.tell()
        # In engine, m_Data points after header+FATs; offsets are relative to that pointer.
        # We converted to absolute offsets relative to file start by applying ~offset relative to m_Data.
        # Here, store absolute file offset = _data_start + (~offset)
        for e in fats:
            e.offset = self._data_start + e.offset
        self.entries = fats

    def close(self) -> None:
        if self._fp:
            self._fp.close()
            self._fp = None

    def list(self) -> List[GscEntry]:
        return list(self.entries)

    def read_file(self, name: str) -> bytes:
        entry = self._find(name)
        return self._read_entry(entry)

    def try_read_palette(self) -> bytes | None:
        # Try resolve a default palette from common files
        candidates = [
            "0\\agew_1.pal", "1\\agew_1.pal", "2\\agew_1.pal", "agew_1.pal",
            "2w.pal"
        ]
        for c in candidates:
            try:
                blob = self.read_file(c)
                # engine reads 256*3 bytes (RGB)
                if len(blob) >= 768:
                    return blob[:768]
            except FileNotFoundError:
                continue
        return None

    def list_palettes(self) -> List[Tuple[str, bytes]]:
        """Return all .PAL files in archive as (name, rgb768)."""
        pals: List[Tuple[str, bytes]] = []
        for e in self.entries:
            if e.name.upper().endswith('.PAL') and e.size >= 768:
                try:
                    blob = self.read_file(e.name)
                    pals.append((e.name, blob[:768]))
                except Exception:
                    continue
        return pals

    def _read_entry(self, entry: GscEntry) -> bytes:
        if not self._fp:
            raise RuntimeError("Archive not open")
        self._fp.seek(entry.offset)
        data = bytearray(self._fp.read(entry.size))
        if entry.flags:
            isi_decrypt_mem(data, self.key_byte)
        return bytes(data)

    def _find(self, name: str) -> GscEntry:
        up = name.replace("/", "\\").upper()
        for e in self.entries:
            if e.name == up:
                return e
            # also allow matching by basename
            if e.name.split("\\")[-1] == up:
                return e
        raise FileNotFoundError(name)


@dataclass
class BuildItem:
    name: str
    data: bytes
    encrypted: bool = False


def build_gsc(items: Iterable[BuildItem], key_word: int = 0x78CD) -> bytes:
    # Build a new GSC in-memory (read-only mapping like engine). Offsets are stored as ~relative
    items_list = list(items)
    entries = len(items_list)
    header = GSC_HEADER_STRUCT.pack(b"GSCRES", 0x0100, key_word, entries)
    # Prepare FATs (with placeholder offsets)
    fats: List[Tuple[int, bytes, int, int, int, int]] = []
    for it in items_list:
        nm = normalize_name(it.name)
        h = calc_hash(nm)
        fats.append((h, nm, 0, len(it.data), 0, 1 if it.encrypted else 0))
    fat_blob = bytearray()
    for h, nm, off, sz, resv, flg in fats:
        fat_blob += GSC_FAT_STRUCT.pack(h, nm, off, sz, resv, flg)

    # Data region
    data_region = bytearray()
    # For each item, record current offset relative to data start, then store ~offset in FAT
    rel_offsets: List[int] = []
    key_byte = (~((key_word >> 8) & 0xFF)) & 0xFF
    for it in items_list:
        rel_off = len(data_region)
        rel_offsets.append(rel_off)
        blob = bytearray(it.data)
        if it.encrypted:
            # Store cipher so engine's MemDecrypt (~byte ^ (~hibyte)) yields plaintext
            key_hi = (key_word >> 8) & 0xFF
            isi_encrypt_mem(blob, key_hi)
        data_region += blob

    # Write inverted offsets
    fat_view = memoryview(fat_blob)
    for idx, rel_off in enumerate(rel_offsets):
        # locate the off field inside record idx
        base = idx * GSC_FAT_STRUCT.size
        # fields: I (hash) 64s (name) I (off) I (size) I (resv) b (flags)
        off_field = base + 4 + 64
        inv = (~rel_off) & 0xFFFFFFFF
        fat_view[off_field : off_field + 4] = struct.pack("<I", inv)

    return bytes(header + fat_blob + data_region)


