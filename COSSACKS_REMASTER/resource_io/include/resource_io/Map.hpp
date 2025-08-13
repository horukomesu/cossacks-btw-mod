#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace resource_io::map {

struct ChunkPresence {
    std::uint32_t id{0};
    std::uint32_t size{0};
};

// Minimal terrain/map data loaded from original PMD3 (.m3d) format
// Mirrors key parts used by the original engine: heights (THMap) and texture indices (TexMap)
struct MapData {
    // Scale exponent from header (ADDSH in original): 1..3
    int addShift{1};
    // World dimensions in tiles (msx, msy in original)
    int mapTilesX{0};
    int mapTilesY{0};
    // Vertex grid parameters from header (computed from addShift)
    int maxTH{0};         // MaxSector*2
    int vertInLine{0};    // MaxSector*2 + 1

    // Height map in vertices (size: (maxTH+1) * maxTH), short (little-endian) in source
    std::vector<int16_t> heights;
    // Texture indices per vertex (size: (maxTH+1) * maxTH), bytes in source
    std::vector<uint8_t> texIndices;
    // Water depth/brightness maps (optional, read from '2AES'), layout: (MaxWX) stride, sizes (msx+2) x (msy+2)
    std::vector<uint8_t> waterDeep;     // (msx+2)*(msy+2)
    std::vector<uint8_t> waterBright;   // (msx+2)*(msy+2)
    // Topology (optional): areas, links and refs as raw blobs for now; detailed structures can be modeled later
    std::vector<uint8_t> topLand;  // concatenated payload of '1POT' or '1POT' simplified variant
    std::vector<uint8_t> topWater; // 'WPOT'

    // Derived visuals for convenience
    std::vector<uint8_t> minimapRGBA; // w=msx/2, h=msy/2, RGBA8888
    std::vector<uint8_t> vertexLight; // per-vertex light [0..255], size (vertInLine*maxTH)

    // Water dims (from '2AES') for diagnostics
    int waterLx{0};
    int waterLy{0};

    // Raw chunk presence for diagnostics
    std::vector<ChunkPresence> chunks;
};

// Load original PMD3 map (.m3d). Returns true on success. Searches archives and data root.
bool load_m3d(const std::string& name, MapData& out);

// Log a concise summary of loaded map data and referenced textures to stdout
void log_map_summary(const MapData& md);

// Full scene representation of PMD3 (.m3d) with one-to-one chunk coverage.
// All structures mirror the original binary layout and semantics.
struct MapSceneData {
    // Header-derived parameters
    int addShift{1};
    int mapTilesX{0};
    int mapTilesY{0};
    int maxTH{0};
    int vertInLine{0};
    int maxSector{0};

    // Chunk presence for diagnostics
    std::vector<ChunkPresence> chunks;

    // WEIV: 292x190 preview (optional)
    std::vector<uint8_t> previewWEIV; // size 292*190 when present

    // FRUS/ELIT: heights and tile indices
    std::vector<int16_t> heights;     // (vertInLine)*(maxTH)
    std::vector<uint8_t> texIndices;  // same element count as heights

    // TCES: section map
    std::vector<uint8_t> sectMap;     // size = maxSector*maxTH*6

    // EERT: static decor sprites
    struct DecorSprite { uint16_t sign; int32_t x; int32_t y; uint16_t sgIndex; };
    std::vector<DecorSprite> decorSprites;

    // Units (multiple versions possible; whichever chunk is present will be populated)
    struct UnitV1 { uint8_t NI; uint16_t NIndex; int32_t x; int32_t y; uint16_t life; uint16_t stage; char name[33]; };
    struct UnitV2 : UnitV1 { int16_t wx; int16_t wy; };
    struct UnitV3 : UnitV2 { uint8_t dir; uint8_t opt; };
    std::vector<UnitV1> unitsV1;
    std::vector<UnitV2> unitsV2;
    std::vector<UnitV3> unitsV3;

    // Walls (three versions)
    struct WallCellV0 { int16_t x; int16_t y; uint8_t type; uint8_t NI; uint8_t stage; uint8_t maxStage; uint16_t health; uint16_t maxHealth; uint8_t sprite; uint8_t sprBase; uint16_t clusterIndex; };
    struct WallCellV1 : WallCellV0 { uint8_t visible; };
    struct WallCellV2 : WallCellV1 { uint16_t gateIndex; uint32_t locks; uint8_t dirMask; uint16_t oIndex; };
    struct WallClusterHeader { uint8_t type; uint32_t nCells; uint32_t nmIndex; };
    struct WallClusterV0 { WallClusterHeader hdr; std::vector<WallCellV0> cells; };
    struct WallClusterV1 { WallClusterHeader hdr; std::vector<WallCellV1> cells; };
    struct WallClusterV2 { WallClusterHeader hdr; std::vector<WallCellV2> cells; };
    std::vector<WallClusterV0> wallsV0; // 'LLAW'
    std::vector<WallClusterV1> wallsV1; // '1LAW'
    std::vector<WallClusterV2> wallsV2; // '2LAW'

    // 1COL: LockBars/UnLockBars
    struct BlockCell { uint16_t x; uint16_t y; };
    std::vector<BlockCell> lockBars;
    std::vector<BlockCell> unlockBars;

    // 2AES: water cost (Deep/Bright)
    int waterLx{0};
    int waterLy{0};
    std::vector<uint8_t> waterDeep;   // size Lx*Ly
    std::vector<uint8_t> waterBright; // size Lx*Ly

    // USER: resource matrix 8x8 of int32
    int32_t RES8x8[8][8]{};

    // 1POT/WPOT: land/water topology
    #pragma pack(push, 1)
    struct StrategyInfoDisk { uint16_t buildInfo; uint8_t nPeasants; uint8_t nShortRange; uint8_t nLongRange; uint8_t nMortir; uint8_t nTowers; uint8_t nPushek; };
    struct AreaDisk32 {
        int16_t x; int16_t y; uint8_t importance; uint8_t nTrees; uint8_t nStones;
        uint16_t nMines; uint32_t minesIdxPtr; uint16_t nLinks; uint32_t linkPtr; uint16_t maxLink;
        StrategyInfoDisk sinf[8];
    };
    #pragma pack(pop)
    struct AreaRuntime { int16_t x; int16_t y; uint8_t importance; uint8_t nTrees; uint8_t nStones; std::vector<uint16_t> minesIdx; std::vector<uint16_t> linksPairs; /* pairs of word */ };
    struct Topology { std::vector<AreaRuntime> areas; std::vector<uint16_t> motionLinks; std::vector<uint16_t> linksDist; std::vector<uint16_t> topRef; };
    Topology landTopo;  // '1POT' or old variant
    Topology waterTopo; // 'WPOT'
    // Raw payload copies to ensure byte-exact round-trip for pointer-bearing structures
    std::vector<uint8_t> landTopoRaw;
    std::vector<uint8_t> waterTopoRaw;

    // 1TAG: Gates
    struct Gate { int16_t x; int16_t y; uint8_t NI; uint8_t nMask; uint8_t state; uint8_t delay; uint8_t locked; uint8_t charId; };
    int nGates{0};
    int maxGates{0};
    std::vector<Gate> gates;

    // 1NOZ: Zones/Groups — full binary payload retained for round-trip (parsing optional)
    std::vector<uint8_t> zonesGroupsRaw;

    // MROF/1ROF: Formations — raw payload retained for round-trip
    std::vector<uint8_t> formationsRaw;

    // LLD.: MapScenaryDLL
    char mapScenaryDLL[200]{};

    // AIIA: AI parameters
    int32_t RM_LandType{0};
    int32_t RM_Resstart{0};
    int32_t RM_Restot{0};

    // SMSP: samples — raw
    std::vector<uint8_t> smsRaw;

    // PEAC: peacetime and flags — raw
    std::vector<uint8_t> peacetimeRaw;

    // 1VIR: rivers
    std::vector<uint8_t> riversDir;  // RivNX*RivNX
    std::vector<uint8_t> riversVol;  // RivNX*RivNX
};

// Load full scene from .m3d (all chunks one-to-one). Returns true on success.
bool load_m3d_scene(const std::string& name, MapSceneData& out);

// Save full scene to .m3d (strict original order and binary layout).
bool save_m3d_scene(const std::string& name, const MapSceneData& scene);

} // namespace resource_io::map


