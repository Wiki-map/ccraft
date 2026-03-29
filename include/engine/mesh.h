#pragma once

#include <cstdint>
#include <array>

#include "glm/glm.hpp"

#include "engine/texture.h"
#include "utils.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    float tex_index;
};

struct Mesh {

    Mesh() = default;
    Mesh(int32_t max_size);

    void UpdeteMesh();
    void Clear();

    void PushTriangleGLM(
        glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,
        glm::vec2 uv1,glm::vec2 uv2,glm::vec2 uv3,
        Texture tex
    );

    void PushTriangle(
        vec3 p1,vec3 p2,vec3 p3,
        vec2 uv1,vec2 uv2,vec2 uv3,
        Texture tex
    );

    void Draw();

    // TODO: check how to delete a buffer
    ~Mesh();

private:

    uint32_t vao,vbo;

    Vertex* vertices;
    int32_t triangle_count;
    int32_t max_triangle_count;

    std::array<uint32_t,8> textures;
    int32_t texture_count;
};