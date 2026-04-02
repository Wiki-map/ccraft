#pragma once

#include <cstdint>
#include <array>

#include "engine/texture.h"
#include "utils.h"

struct Vertex {
    vec3 pos;
    vec2 uv;
    int32_t tex_index;
    vec3 norm;
    float tint;
};

struct Mesh {

    Mesh() = default;
    Mesh(int32_t max_size);

    void UpdeteMesh();
    void Clear();

    // NOTE: tint is a float from 0 to 1 specifing the darkness, used for cpu AO
    void PushTriangle(
        vec3 p1,vec3 p2,vec3 p3,
        vec2 uv1,vec2 uv2,vec2 uv3,
        Texture tex,
        vec3 norm,float tint
    );

    void Draw();

    void Clean();

private:

    uint32_t vao,vbo;

    Vertex* vertices;
    int32_t triangle_count;
    int32_t max_triangle_count;

    std::array<uint32_t,8> textures;
    int32_t texture_count;
};
