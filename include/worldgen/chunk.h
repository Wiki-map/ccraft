#pragma once

#include "utils.h"
#include "engine/mesh.h"
#include <functional>


#define CHUNK_SIZE 16

struct Chunk {

    Chunk() = default;
    Chunk(vec2 pos);

    void Generate(std::function<float(float,float)> noise_fun);

    void Draw();

    void Clean();

private:
    Mesh mesh;
    vec2 position;
};
