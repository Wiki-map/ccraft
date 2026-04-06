#pragma once

#include "utils.h"
#include <vector>
#include "worldgen/chunk.h"

struct ChunkManager {
    ChunkManager(int render_distance);
    void Update(vec3 player_pos);
    void Draw();
    void Clean();
    BlockType GetBlock(vec3 pos);
    vec2 GetChunk(vec3 pos);
    void GenChunk(int i,int j,std::function<BlockType(vec3)> sampler);
private:

    vec2 postion;
    vec2 last_player_position;
    int render_distance;
    std::vector<std::vector<Chunk>> chunks;
    std::vector<std::vector<bool>> isGenerated;
};
