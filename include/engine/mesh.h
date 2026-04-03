#pragma once

#include <cstdint>
#include "engine/color.h"
#include "utils.h"

struct Vertex {
    vec3 pos;
    vec4 color;
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
        Color color,
        vec3 norm,
        float tint
    );

    void Draw();

    void Clean();

private:

    uint32_t vao,vbo;

    Vertex* vertices;
    int32_t triangle_count;
    int32_t max_triangle_count;
};
