#pragma once

#include "utils.h"
#include "engine/mesh.h"
#include "worldgen/extern/FastNoise.h"

#define CHUNK_SIZE 16

struct Chunk {

    Chunk() = default;
    Chunk(vec2 pos);

    void Generate(FastNoise noise_maker,Texture tex);

    void Draw();

private:
    Mesh chunk_mesh;
    vec2 position;
};
