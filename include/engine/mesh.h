#pragma once

#include <cstdint>
#include <vector>
#include "engine/color.h"
#include "cgm.h"

enum class NormalDir {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

struct Mesh {

    struct Vertex {
        vec3 pos;
        uint32_t color;
        uint8_t norm; // thecnicly 3bits
    };

    Mesh() = default;

    void Init();

    void UpdeteMesh();
    void Clear();

    void PushTriangle(
        vec3 p1,vec3 p2,vec3 p3,
        Color color,
        NormalDir norm
    );

    /*
     *  top --- right
     *  |         |
     *  |         |
     *  |         |
     *  left --- bot
     */
    void PushQuad(
        vec3 top,vec3 right, vec3 left, vec3 bot,
        Color color,
        NormalDir norm
    );

    void Draw();

    void Clean();

private:

    uint32_t vao,vbo;

    std::vector<Vertex> vertices;
    int32_t triangle_count;
};
