#pragma once

#include "utils.h"
#include <vector>
#include <functional>
#include "worldgen/chunk.h"

struct ChunkManager {
    ChunkManager(int render_distance);
    void SetNoiseFuction(std::function<float(float,float)> fun);
    void Update(vec3 player_pos);
    void Draw();
    void Clean();
    BlockType GetBlock(vec3 pos);
    vec2 GetChunk(vec3 pos);
private:

    void ShiftMap(int diffi,int diffj);
    bool TryGen();
    bool TryMesh();

    vec2 postion;
    vec2 last_player_position;
    int render_distance;
    int render_distance_p;
    std::function<float(float,float)> noise;
    std::vector<std::vector<Chunk>> chunks;
    std::vector<std::vector<bool>> isGenerated;
    std::vector<std::vector<bool>> isMeshGenerated;
};
