#pragma once

#include "utils.h"
#include "engine/mesh.h"
#include <functional>
#include <array>


#define CHUNK_SIZE 16

struct Chunk {

    Chunk() = default;
    Chunk(vec2 pos);

    void GenerateHeight(std::function<float(float,float)> noise_fun);
    void GenerateMesh(std::function<int32_t(float,float)> height_sampler);

    void Draw();

    void Clean();

    int32_t GetHeight(float i,float j);

private:

    void SpawnBlock(vec3 pos,std::function<int32_t(float,float)> height_sampler);

    Mesh mesh;
    vec2 position;

    std::array<std::array<int32_t,CHUNK_SIZE>,CHUNK_SIZE> height;
};
