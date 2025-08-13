#include "engine_core/Render2D.hpp"

#include <glad/gl.h>
#include <array>
#include <string>

namespace engine_core::render2d {

namespace {
GLuint gProgram = 0;
GLuint gVao = 0;
GLuint gVbo = 0;
GLint gLocProj = -1;
GLint gLocColor = -1;

GLuint gTexProgram = 0;
GLuint gTexVao = 0;
GLuint gTexVbo = 0;
GLint gTexLocProj = -1;
GLint gTexLocSampler = -1;
GLint gTexLocTint = -1;

// Terrain (indexed + palette + masks) pipeline
GLuint gTerrProgram = 0;
GLuint gTerrVao = 0;
GLuint gTerrVbo = 0;
GLint gTerrLocProj = -1;
// samplers
GLint gTerrLocAtlas = -1;
GLint gTerrLocPalette = -1;
GLint gTerrLocTrans4 = -1;
GLint gTerrLocTrans8 = -1;
GLint gTerrLocMaskA = -1;
GLint gTerrLocMaskB = -1;
GLint gTerrLocMaskC = -1;
GLint gTerrLocMaskD = -1;
// params
GLint gTerrLocAtlasSize = -1;
GLint gTerrLocTileSize = -1;
GLint gTerrLocBaseTileIndex = -1;
GLint gTerrLocMaskOrigin = -1;
GLint gTerrLocNumOverlays = -1;
GLint gTerrLocOverlayTileIndex = -1;
GLint gTerrLocOverlayMaskId = -1;
GLint gTerrLocTint = -1;
GLint gTerrLocMaskALayers = -1;
GLint gTerrLocMaskBLayers = -1;
GLint gTerrLocMaskCLayers = -1;
GLint gTerrLocMaskDLayers = -1;

// resources bound from terrain
GLuint gTerrAtlasIdxTex = 0;
GLuint gTerrPaletteTex = 0;
GLuint gTerrTrans4Tex = 0;
GLuint gTerrTrans8Tex = 0;
GLuint gTerrMaskATex = 0;
GLuint gTerrMaskBTex = 0;
GLuint gTerrMaskCTex = 0;
GLuint gTerrMaskDTex = 0;
int gTerrAtlasW = 0;
int gTerrAtlasH = 0;
int gTerrTileW = 64;
int gTerrTileH = 64;

void destroy() {
    if (gVbo) { glDeleteBuffers(1, &gVbo); gVbo = 0; }
    if (gVao) { glDeleteVertexArrays(1, &gVao); gVao = 0; }
    if (gProgram) { glDeleteProgram(gProgram); gProgram = 0; }
    if (gTexVbo) { glDeleteBuffers(1, &gTexVbo); gTexVbo = 0; }
    if (gTexVao) { glDeleteVertexArrays(1, &gTexVao); gTexVao = 0; }
    if (gTexProgram) { glDeleteProgram(gTexProgram); gTexProgram = 0; }
    if (gTerrVbo) { glDeleteBuffers(1, &gTerrVbo); gTerrVbo = 0; }
    if (gTerrVao) { glDeleteVertexArrays(1, &gTerrVao); gTerrVao = 0; }
    if (gTerrProgram) { glDeleteProgram(gTerrProgram); gTerrProgram = 0; }
}

GLuint compile(GLenum type, const char* src) {
    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &src, nullptr);
    glCompileShader(sh);
    GLint ok = 0; glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    return sh;
}

GLuint link(GLuint vs, GLuint fs) {
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    GLint ok = 0; glGetProgramiv(p, GL_LINK_STATUS, &ok);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

} // namespace

bool initialize() {
    const char* vsSrc = R"GLSL(
        #version 330 core
        layout(location=0) in vec2 aPos;
        uniform mat4 uProj;
        void main(){ gl_Position = uProj * vec4(aPos, 0.0, 1.0); }
    )GLSL";
    const char* fsSrc = R"GLSL(
        #version 330 core
        uniform vec4 uColor;
        out vec4 FragColor;
        void main(){ FragColor = uColor; }
    )GLSL";
    GLuint vs = compile(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fsSrc);
    gProgram = link(vs, fs);

    glGenVertexArrays(1, &gVao);
    glGenBuffers(1, &gVbo);

    // Textured pipeline
    const char* tvsSrc = R"GLSL(
        #version 330 core
        layout(location=0) in vec2 aPos;
        layout(location=1) in vec2 aUV;
        uniform mat4 uProj;
        out vec2 vUV;
        void main(){ vUV = aUV; gl_Position = uProj * vec4(aPos, 0.0, 1.0); }
    )GLSL";
    const char* tfsSrc = R"GLSL(
        #version 330 core
        in vec2 vUV;
        uniform sampler2D uTex;
        uniform vec4 uTint;
        out vec4 FragColor;
        void main(){ FragColor = texture(uTex, vUV) * uTint; }
    )GLSL";
    GLuint tvs = compile(GL_VERTEX_SHADER, tvsSrc);
    GLuint tfs = compile(GL_FRAGMENT_SHADER, tfsSrc);
    gTexProgram = link(tvs, tfs);

    glGenVertexArrays(1, &gTexVao);
    glGenBuffers(1, &gTexVbo);

    // Terrain pipeline (indexed tiles + palette + masks on GPU)
    const char* t2vsSrc = R"GLSL(
        #version 330 core
        layout(location=0) in vec2 aPos;
        layout(location=1) in vec2 aUV;
        uniform mat4 uProj;
        out vec2 vUV;
        void main(){ vUV = aUV; gl_Position = uProj * vec4(aPos, 0.0, 1.0); }
    )GLSL";
    const char* t2fsSrc = R"GLSL(
        #version 330 core
        in vec2 vUV;
        uniform sampler2D uAtlasIdx;      // R8
        uniform sampler2D uPalette;       // RGBA8 256x1
        uniform sampler2D uTrans4;        // R8 256x256
        uniform sampler2D uTrans8;        // R8 256x256
        uniform sampler2DArray uMaskA;    // R8 64x64xN
        uniform sampler2DArray uMaskB;    // R8 64x64xN
        uniform sampler2DArray uMaskC;    // R8 64x64xN
        uniform sampler2DArray uMaskD;    // R8 64x64xN
        uniform int uMaskALayers;
        uniform int uMaskBLayers;
        uniform int uMaskCLayers;
        uniform int uMaskDLayers;
        uniform ivec2 uAtlasSize;         // pixels
        uniform ivec2 uTileSize;          // 64,64
        uniform int uBaseTileIndex;
        uniform ivec2 uMaskOrigin;        // x0,y0 [0..63]
        uniform int uBaseYOffset;         // 0 for type1, -31 for type2 parity
        uniform int uNumOverlays;
        uniform int uOverlayTileIndex[2];
        uniform int uOverlayMaskId[2];
        uniform vec4 uTint;
        out vec4 FragColor;

        // integer division helpers
        ivec2 tileOrigin(int tileIndex){
            int tilesX = uAtlasSize.x / uTileSize.x;
            int u = tileIndex % tilesX;
            int v = tileIndex / tilesX;
            int x = u * uTileSize.x;
            int yTop = v * uTileSize.y;
            // convert top-left y to OpenGL row index (bottom-left origin)
            int y = (uAtlasSize.y - 1) - yTop; // points to start of tile's top row mapped to GL top
            // After this conversion, texelFetch needs y' = y - localY
            return ivec2(x, y);
        }

        int sampleIndexFromTile(int tileIndex, ivec2 local){
            ivec2 o = tileOrigin(tileIndex);
            // local coordinates modulo tile size [0..tile-1]
            int lx = (local.x % uTileSize.x + uTileSize.x) % uTileSize.x;
            int ly = (local.y % uTileSize.y + uTileSize.y) % uTileSize.y;
            // GL bottom-left origin: move downwards for increasing ly
            int x = o.x + lx;
            int y = o.y - ly;
            float idx = texelFetch(uAtlasIdx, ivec2(x, y), 0).r * 255.0;
            return int(idx + 0.5);
        }

        int trans4(int dest, int src){
            float v = texelFetch(uTrans4, ivec2(src, dest), 0).r * 255.0;
            return int(v + 0.5);
        }
        int trans8(int dest, int src){
            float v = texelFetch(uTrans8, ivec2(src, dest), 0).r * 255.0;
            return int(v + 0.5);
        }

        bool maskOn(sampler2DArray m, ivec2 coord, int layer, int layers){
            if (layer < 0 || layer >= layers) return false;
            // coord within 64x64, layer is mask id
            float mv = texelFetch(m, ivec3(coord, layer), 0).r;
            return mv > 0.0;
        }

        int applyOverlay(int destIdx, int overlayTile, int maskId, ivec2 maskCoord){
            int srcIdx = sampleIndexFromTile(overlayTile, maskCoord);
            // A layer: copy
            if (maskOn(uMaskA, maskCoord, maskId, uMaskALayers)) destIdx = srcIdx;
            // B layer: trans4(dest, src)
            if (maskOn(uMaskB, maskCoord, maskId, uMaskBLayers)) destIdx = trans4(destIdx, srcIdx);
            // C layer: trans8(dest, src)
            if (maskOn(uMaskC, maskCoord, maskId, uMaskCLayers)) destIdx = trans8(destIdx, srcIdx);
            // D layer: trans4(src, dest)
            if (maskOn(uMaskD, maskCoord, maskId, uMaskDLayers)) destIdx = trans4(srcIdx, destIdx);
            return destIdx;
        }

        void main(){
            // local pixel coordinate within 64x64 triangle
            ivec2 uvPix = ivec2(clamp(floor(vUV * vec2(uTileSize)), vec2(0.0), vec2(uTileSize) - 1.0));
            // base sampling uses mask origin offset for parity with CPU fill
            ivec2 baseCoord = ivec2((uMaskOrigin.x + uvPix.x) & (uTileSize.x - 1), (uMaskOrigin.y + uvPix.y + uBaseYOffset) & (uTileSize.y - 1));
            int destIdx = sampleIndexFromTile(uBaseTileIndex, baseCoord);
            // overlays: same mask coord basis
            ivec2 maskCoord = baseCoord;
            if (uNumOverlays > 0){
                destIdx = applyOverlay(destIdx, uOverlayTileIndex[0], uOverlayMaskId[0], maskCoord);
            }
            if (uNumOverlays > 1){
                destIdx = applyOverlay(destIdx, uOverlayTileIndex[1], uOverlayMaskId[1], maskCoord);
            }
            // palette lookup
            vec4 color = texelFetch(uPalette, ivec2(destIdx, 0), 0);
            FragColor = color * uTint;
        }
    )GLSL";
    GLuint t2vs = compile(GL_VERTEX_SHADER, t2vsSrc);
    GLuint t2fs = compile(GL_FRAGMENT_SHADER, t2fsSrc);
    gTerrProgram = link(t2vs, t2fs);
    glGenVertexArrays(1, &gTerrVao);
    glGenBuffers(1, &gTerrVbo);

    return gProgram != 0 && gTexProgram != 0 && gTerrProgram != 0;
}

void shutdown() { destroy(); }

void begin_frame(int framebufferWidth, int framebufferHeight) {
    // Enable alpha blending for textures with transparency (cursor, UI)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Color pipeline
    glUseProgram(gProgram);
    glBindVertexArray(gVao);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)0);
    const float L = 0.0f, T = 0.0f, R = static_cast<float>(framebufferWidth), B = static_cast<float>(framebufferHeight);
    const float proj[16] = {
        2.0f/(R-L), 0, 0, 0,
        0, -2.0f/(B-T), 0, 0,
        0, 0, -1, 0,
        -(R+L)/(R-L), (B+T)/(B-T), 0, 1
    };
    gLocProj = glGetUniformLocation(gProgram, "uProj");
    gLocColor = glGetUniformLocation(gProgram, "uColor");
    glUniformMatrix4fv(gLocProj, 1, GL_FALSE, proj);

    // Textured pipeline
    glUseProgram(gTexProgram);
    glBindVertexArray(gTexVao);
    glBindBuffer(GL_ARRAY_BUFFER, gTexVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));
    gTexLocProj = glGetUniformLocation(gTexProgram, "uProj");
    gTexLocSampler = glGetUniformLocation(gTexProgram, "uTex");
    gTexLocTint = glGetUniformLocation(gTexProgram, "uTint");
    glUniformMatrix4fv(gTexLocProj, 1, GL_FALSE, proj);
    glUniform1i(gTexLocSampler, 0);
    glUniform4f(gTexLocTint, 1.0f, 1.0f, 1.0f, 1.0f);

    // Terrain pipeline uniforms (guard program presence)
    if (gTerrProgram) {
        glUseProgram(gTerrProgram);
        glBindVertexArray(gTerrVao);
        glBindBuffer(GL_ARRAY_BUFFER, gTerrVbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));
        gTerrLocProj = glGetUniformLocation(gTerrProgram, "uProj");
        gTerrLocAtlas = glGetUniformLocation(gTerrProgram, "uAtlasIdx");
        gTerrLocPalette = glGetUniformLocation(gTerrProgram, "uPalette");
        gTerrLocTrans4 = glGetUniformLocation(gTerrProgram, "uTrans4");
        gTerrLocTrans8 = glGetUniformLocation(gTerrProgram, "uTrans8");
        gTerrLocMaskA = glGetUniformLocation(gTerrProgram, "uMaskA");
        gTerrLocMaskB = glGetUniformLocation(gTerrProgram, "uMaskB");
        gTerrLocMaskC = glGetUniformLocation(gTerrProgram, "uMaskC");
        gTerrLocMaskD = glGetUniformLocation(gTerrProgram, "uMaskD");
        gTerrLocAtlasSize = glGetUniformLocation(gTerrProgram, "uAtlasSize");
        gTerrLocTileSize = glGetUniformLocation(gTerrProgram, "uTileSize");
        gTerrLocBaseTileIndex = glGetUniformLocation(gTerrProgram, "uBaseTileIndex");
        gTerrLocMaskOrigin = glGetUniformLocation(gTerrProgram, "uMaskOrigin");
        gTerrLocMaskALayers = glGetUniformLocation(gTerrProgram, "uMaskALayers");
        gTerrLocMaskBLayers = glGetUniformLocation(gTerrProgram, "uMaskBLayers");
        gTerrLocMaskCLayers = glGetUniformLocation(gTerrProgram, "uMaskCLayers");
        gTerrLocMaskDLayers = glGetUniformLocation(gTerrProgram, "uMaskDLayers");
        gTerrLocNumOverlays = glGetUniformLocation(gTerrProgram, "uNumOverlays");
        gTerrLocOverlayTileIndex = glGetUniformLocation(gTerrProgram, "uOverlayTileIndex");
        gTerrLocOverlayMaskId = glGetUniformLocation(gTerrProgram, "uOverlayMaskId");
        gTerrLocTint = glGetUniformLocation(gTerrProgram, "uTint");
        glUniformMatrix4fv(gTerrLocProj, 1, GL_FALSE, proj);
        // bind sampler units indices
        glUniform1i(gTerrLocAtlas, 0);
        glUniform1i(gTerrLocPalette, 1);
        glUniform1i(gTerrLocTrans4, 2);
        glUniform1i(gTerrLocTrans8, 3);
        glUniform1i(gTerrLocMaskA, 4);
        glUniform1i(gTerrLocMaskB, 5);
        glUniform1i(gTerrLocMaskC, 6);
        glUniform1i(gTerrLocMaskD, 7);
    }

    glUseProgram(0);
}

void end_frame() {
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void draw_rect(float x, float y, float w, float h, float r, float g, float b, float a) {
    const std::array<float, 12> verts = {
        x, y,
        x+w, y,
        x+w, y+h,
        x, y,
        x+w, y+h,
        x, y+h
    };
    glUseProgram(gProgram);
    glBindVertexArray(gVao);
    glBindBuffer(GL_ARRAY_BUFFER, gVbo);
    glUniform4f(gLocColor, r, g, b, a);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void draw_textured_quad(unsigned int texture, float x, float y, float w, float h) {
    const std::array<float, 24> verts = {
        // pos      // uv
        x,   y,     0.0f, 0.0f,
        x+w, y,     1.0f, 0.0f,
        x+w, y+h,   1.0f, 1.0f,
        x,   y,     0.0f, 0.0f,
        x+w, y+h,   1.0f, 1.0f,
        x,   y+h,   0.0f, 1.0f
    };
    glUseProgram(gTexProgram);
    glBindVertexArray(gTexVao);
    glBindBuffer(GL_ARRAY_BUFFER, gTexVbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_textured_quad_tinted(unsigned int texture, float x, float y, float w, float h,
                               float r, float g, float b, float a) {
    const std::array<float, 24> verts = {
        // pos      // uv
        x,   y,     0.0f, 0.0f,
        x+w, y,     1.0f, 0.0f,
        x+w, y+h,   1.0f, 1.0f,
        x,   y,     0.0f, 0.0f,
        x+w, y+h,   1.0f, 1.0f,
        x,   y+h,   0.0f, 1.0f
    };
    glUseProgram(gTexProgram);
    glBindVertexArray(gTexVao);
    glBindBuffer(GL_ARRAY_BUFFER, gTexVbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform4f(gTexLocTint, r, g, b, a);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniform4f(gTexLocTint, 1.0f, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_textured_quad_region_tinted(unsigned int texture, float x, float y, float w, float h,
                                      float u0, float v0, float u1, float v1,
                                      float r, float g, float b, float a) {
    const std::array<float, 24> verts = {
        // pos      // uv
        x,   y,     u0, v0,
        x+w, y,     u1, v0,
        x+w, y+h,   u1, v1,
        x,   y,     u0, v0,
        x+w, y+h,   u1, v1,
        x,   y+h,   u0, v1
    };
    glUseProgram(gTexProgram);
    glBindVertexArray(gTexVao);
    glBindBuffer(GL_ARRAY_BUFFER, gTexVbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform4f(gTexLocTint, r, g, b, a);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUniform4f(gTexLocTint, 1.0f, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_textured_triangle_tinted(unsigned int texture,
                                   float x1, float y1, float u1, float v1,
                                   float x2, float y2, float u2, float v2,
                                   float x3, float y3, float u3, float v3,
                                   float r, float g, float b, float a) {
    const std::array<float, 12> posuv1 = {
        x1, y1, u1, v1,
        x2, y2, u2, v2,
        x3, y3, u3, v3
    };
    glUseProgram(gTexProgram);
    glBindVertexArray(gTexVao);
    glBindBuffer(GL_ARRAY_BUFFER, gTexVbo);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform4f(gTexLocTint, r, g, b, a);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posuv1), posuv1.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUniform4f(gTexLocTint, 1.0f, 1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void set_terrain_gpu_resources(unsigned int atlasIndexTex,
                               unsigned int paletteTex,
                               unsigned int trans4Tex,
                               unsigned int trans8Tex,
                               unsigned int maskATexArray,
                               unsigned int maskBTexArray,
                               unsigned int maskCTexArray,
                               unsigned int maskDTexArray,
                               int atlasWidth,
                               int atlasHeight,
                               int maskALayers,
                               int maskBLayers,
                               int maskCLayers,
                               int maskDLayers) {
    gTerrAtlasIdxTex = atlasIndexTex;
    gTerrPaletteTex = paletteTex;
    gTerrTrans4Tex = trans4Tex;
    gTerrTrans8Tex = trans8Tex;
    gTerrMaskATex = maskATexArray;
    gTerrMaskBTex = maskBTexArray;
    gTerrMaskCTex = maskCTexArray;
    gTerrMaskDTex = maskDTexArray;
    gTerrAtlasW = atlasWidth;
    gTerrAtlasH = atlasHeight;
    // Preload layer counts into uniforms next frame
    if (gTerrProgram) {
        glUseProgram(gTerrProgram);
        if (gTerrLocMaskALayers>=0) glUniform1i(gTerrLocMaskALayers, maskALayers);
        if (gTerrLocMaskBLayers>=0) glUniform1i(gTerrLocMaskBLayers, maskBLayers);
        if (gTerrLocMaskCLayers>=0) glUniform1i(gTerrLocMaskCLayers, maskCLayers);
        if (gTerrLocMaskDLayers>=0) glUniform1i(gTerrLocMaskDLayers, maskDLayers);
        glUseProgram(0);
    }
}

void draw_terrain_triangle_indexed(float x1, float y1, float u1, float v1,
                                   float x2, float y2, float u2, float v2,
                                   float x3, float y3, float u3, float v3,
                                   int baseTileIndex,
                                   int maskOriginX, int maskOriginY,
                                   int baseYOffset,
                                   int numOverlays,
                                   int overlay0TileIndex, int overlay0MaskId,
                                   int overlay1TileIndex, int overlay1MaskId,
                                   float r, float g, float b, float a) {
    if (!gTerrProgram || !gTerrAtlasIdxTex || !gTerrPaletteTex) {
        // Terrain GPU not ready; skip this triangle
        return;
    }
    const std::array<float, 12> posuv = { x1, y1, u1, v1,
                                          x2, y2, u2, v2,
                                          x3, y3, u3, v3 };
    glUseProgram(gTerrProgram);
    glBindVertexArray(gTerrVao);
    glBindBuffer(GL_ARRAY_BUFFER, gTerrVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posuv), posuv.data(), GL_DYNAMIC_DRAW);
    // bind textures
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gTerrAtlasIdxTex);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gTerrPaletteTex);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gTerrTrans4Tex);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gTerrTrans8Tex);
    glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D_ARRAY, gTerrMaskATex);
    glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D_ARRAY, gTerrMaskBTex);
    glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D_ARRAY, gTerrMaskCTex);
    glActiveTexture(GL_TEXTURE7); glBindTexture(GL_TEXTURE_2D_ARRAY, gTerrMaskDTex);
    // uniforms
    glUniform2i(gTerrLocAtlasSize, gTerrAtlasW, gTerrAtlasH);
    glUniform2i(gTerrLocTileSize, gTerrTileW, gTerrTileH);
    glUniform1i(gTerrLocBaseTileIndex, baseTileIndex);
    glUniform2i(gTerrLocMaskOrigin, maskOriginX & 63, maskOriginY & 63);
    GLint loc = glGetUniformLocation(gTerrProgram, "uBaseYOffset");
    glUniform1i(loc, baseYOffset);
    bool masksReady = (gTerrMaskATex && gTerrMaskBTex && gTerrMaskCTex && gTerrMaskDTex);
    int n = numOverlays < 0 ? 0 : (numOverlays > 2 ? 2 : numOverlays);
    if (!masksReady) n = 0;
    glUniform1i(gTerrLocNumOverlays, n);
    int overlayIdx[2] = { overlay0TileIndex, overlay1TileIndex };
    int overlayMask[2] = { overlay0MaskId, overlay1MaskId };
    glUniform1iv(gTerrLocOverlayTileIndex, 2, overlayIdx);
    glUniform1iv(gTerrLocOverlayMaskId, 2, overlayMask);
    glUniform4f(gTerrLocTint, r, g, b, a);
    // draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // unbind not strictly required
}

} // namespace engine_core::render2d
