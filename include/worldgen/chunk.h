#pragma once

#include "engine/color.h"
#include "cgm.h"
#include "engine/mesh.h"
#include <functional>

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 128
#define BLOCK_SIZE (float)1.0f
#define WATTER_LEVEL 25

enum class BlockType {
    AIR,
    DIRT,
    GRASS,
    STONE,
    SAND
};

Color GetBlockColor(BlockType type);

struct Chunk {

    Chunk() = default;
    Chunk(vec2 pos);

    void Init();

    void GenerateVoxels(std::function<float(float,float)> noise_function);
    void GenerateMesh(std::function<BlockType(vec3)> voxel_sampler);

    void Draw();
    void Clean();

    BlockType GetBlock(vec3 pos);

    vec2 GetPosition();

private:

    void PushCube(vec3 top_pos,std::vector<BlockType> &voxelsp,int x,int y,int z);

    Mesh mesh;
    Mesh translucent;
    vec2 position;

    std::vector<BlockType> voxels;
};
