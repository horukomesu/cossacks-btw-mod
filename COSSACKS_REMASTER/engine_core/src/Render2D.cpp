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

void destroy() {
    if (gVbo) { glDeleteBuffers(1, &gVbo); gVbo = 0; }
    if (gVao) { glDeleteVertexArrays(1, &gVao); gVao = 0; }
    if (gProgram) { glDeleteProgram(gProgram); gProgram = 0; }
    if (gTexVbo) { glDeleteBuffers(1, &gTexVbo); gTexVbo = 0; }
    if (gTexVao) { glDeleteVertexArrays(1, &gTexVao); gTexVao = 0; }
    if (gTexProgram) { glDeleteProgram(gTexProgram); gTexProgram = 0; }
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
        out vec4 FragColor;
        void main(){ FragColor = texture(uTex, vUV); }
    )GLSL";
    GLuint tvs = compile(GL_VERTEX_SHADER, tvsSrc);
    GLuint tfs = compile(GL_FRAGMENT_SHADER, tfsSrc);
    gTexProgram = link(tvs, tfs);

    glGenVertexArrays(1, &gTexVao);
    glGenBuffers(1, &gTexVbo);

    return gProgram != 0 && gTexProgram != 0;
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
    glUniformMatrix4fv(gTexLocProj, 1, GL_FALSE, proj);
    glUniform1i(gTexLocSampler, 0);

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

} // namespace engine_core::render2d
