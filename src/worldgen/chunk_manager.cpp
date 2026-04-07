#include "worldgen/chunk_manager.h"
#include "worldgen/chunk.h"
#include "worldgen/extern/FastNoise.h"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

ChunkManager::ChunkManager(int render_distance) {
    this->render_distance = render_distance;
    postion = {-render_distance*CHUNK_SIZE*BLOCK_SIZE,-render_distance*CHUNK_SIZE*BLOCK_SIZE};
    last_player_position = {0,0};
    chunks = std::vector<std::vector<Chunk>>(render_distance*2 + 1,std::vector<Chunk>(render_distance*2 + 1));
    isGenerated = std::vector<std::vector<bool>>(render_distance*2 + 1,std::vector<bool>(render_distance*2 + 1,false));
    tryMeshBuild = std::vector<std::vector<bool>>(render_distance*2 + 1,std::vector<bool>(render_distance*2 + 1,false));
}

vec2 ChunkManager::GetChunk(vec3 pos) {
    vec2 pos2 = {pos.x,pos.z};
    if (pos2.x < 0) pos2.x -= CHUNK_SIZE*BLOCK_SIZE;
    if (pos2.y < 0) pos2.y -= CHUNK_SIZE*BLOCK_SIZE;
    pos2 /= BLOCK_SIZE*CHUNK_SIZE;
    pos2 = {(float)((int)pos2.x),(float)((int)pos2.y)};
    return pos2;
}

BlockType ChunkManager::GetBlock(vec3 pos) {
    if (pos.y < 0 || pos.y >= CHUNK_HEIGHT) return BlockType::AIR;
    if (pos.x < postion.x || pos.x >= postion.x + (render_distance*2+1)*CHUNK_SIZE*BLOCK_SIZE) return BlockType::AIR;
    if (pos.z < postion.y || pos.z >= postion.y + (render_distance*2+1)*CHUNK_SIZE*BLOCK_SIZE) return BlockType::AIR;
    vec2 pos2 = {pos.x,pos.z};
    vec2 rel = pos2 - postion;
    rel.x = (render_distance*2+1)*CHUNK_SIZE*BLOCK_SIZE - rel.x - BLOCK_SIZE;
    vec2 chunkp = GetChunk({rel.x,0,rel.y});
    int i = chunkp.x,j = chunkp.y;
    if (i < 0 || j < 0 || i >= render_distance*2+1 || j >= render_distance*2+1) return BlockType::AIR;
    if (!isGenerated[i][j]) return BlockType::AIR;
    return chunks[i][j].GetBlock(pos);
}

void ChunkManager::ShiftMap(int diffi,int diffj) {
    postion.x += diffi * CHUNK_SIZE * BLOCK_SIZE;
    postion.y += diffj * CHUNK_SIZE * BLOCK_SIZE;

    if (diffi > 0) {
        for (int i=render_distance*2; i>=0; i--) {
            for (int j=0; j<render_distance*2+1; j++) {
                if (i + diffi >= render_distance*2+1) {
                    if (isGenerated[i][j]) chunks[i][j].Clean();
                    continue;
                }
                chunks[i+diffi][j] = std::move(chunks[i][j]);
                isGenerated[i+diffi][j] = isGenerated[i][j];
                tryMeshBuild[i+diffi][j] = tryMeshBuild[i][j];
                if (i < diffi) {
                    isGenerated[i][j] = false;
                }
            }
        }
    }
    if (diffi < 0) {
        diffi = std::abs(diffi);
        for (int i=0; i<render_distance*2+1; i++) {
            for (int j=0; j<render_distance*2+1; j++) {
                if (i - diffi < 0) {
                    if (isGenerated[i][j]) chunks[i][j].Clean();
                    continue;
                }
                chunks[i-diffi][j] = std::move(chunks[i][j]);
                isGenerated[i-diffi][j] = isGenerated[i][j];
                tryMeshBuild[i-diffi][j] = tryMeshBuild[i][j];
                if (i >= render_distance*2+1 - diffi) {
                    isGenerated[i][j] = false;
                }
            }
        }
    }

    if (diffj > 0) {
        for (int j=0; j<render_distance*2+1; j++)  {
            for (int i=0; i<render_distance*2+1; i++)  {
                if (j - diffj < 0) {
                    if (isGenerated[i][j]) chunks[i][j].Clean();
                    continue;
                }
                chunks[i][j-diffj] = std::move(chunks[i][j]);
                isGenerated[i][j-diffj] = isGenerated[i][j];
                tryMeshBuild[i][j-diffi] = tryMeshBuild[i][j];
                if (j >= render_distance*2+1 - diffj) {
                    isGenerated[i][j] = false;
                }
            }
        }

    }

    if (diffj < 0) {
        diffj = std::abs(diffj);
        for (int j=render_distance*2; j>=0; j--) {
            for (int i=0; i<render_distance*2+1; i++) {
                if (j + diffj >= render_distance*2+1) {
                    if (isGenerated[i][j]) chunks[i][j].Clean();
                    continue;
                }
                chunks[i][j+diffj] = std::move(chunks[i][j]);
                isGenerated[i][j+diffj] = isGenerated[i][j];
                tryMeshBuild[i][j + diffi] = tryMeshBuild[i][j];
                if (j < diffj) {
                    isGenerated[i][j] = false;
                }
            }
        }
    }
}

bool ChunkManager::TryGen() {
    FastNoise n;
    n.SetSeed(0);
    n.SetNoiseType(FastNoise::NoiseType::PerlinFractal);
    auto noise = [&](float x,float y) -> float {
        return n.GetPerlinFractal(x,y)*30 + 30;
    };

    auto sampler = [&](vec3 pos) -> BlockType {return GetBlock(pos);};

    auto gen = [&](int i,int j) {
        float x = postion.x + (render_distance*2 - i)*CHUNK_SIZE*BLOCK_SIZE;
        float y = postion.y + j*CHUNK_SIZE*BLOCK_SIZE;
        chunks[i][j] = Chunk({x,y});
        chunks[i][j].Init();
        chunks[i][j].GenerateVoxels(noise);
        isGenerated[i][j] = true;
        chunks[i][j].GenerateMesh(sampler);


        if (i > 0 && isGenerated[i-1][j]) tryMeshBuild[i-1][j] = 1;
        if (j > 0 && isGenerated[i][j-1]) tryMeshBuild[i][j-1] = 1;
        if (i < render_distance*2 && isGenerated[i+1][j]) tryMeshBuild[i+1][j] = 1;
        if (j < render_distance*2 && isGenerated[i][j+1]) tryMeshBuild[i][j-1] = 1;
    };

    bool hasgen = false;
    for (int d=render_distance; d>=0; d--) {
        for (int j=d; j<render_distance*2+1-d; j++) {
            if (!isGenerated[d][j]) {
                gen(d,j);
                hasgen = true;
                break;
            }
            if (!isGenerated[render_distance*2 - d][j]) {
                gen(render_distance*2 -d,j);
                hasgen = true;
                break;
            }
        }
        if (hasgen) break;

        for (int i=d+1; i<render_distance*2 -d; i++) {
            if (!isGenerated[i][d]) {
                gen(i,d);
                hasgen = true;
                break;
            }
            if (!isGenerated[i][render_distance*2-d]) {
                gen(i,render_distance*2-d);
                hasgen = true;
                break;
            }
        }
        if (hasgen) break;
    }
    return hasgen;
}

bool ChunkManager::TryMesh() {
    bool hasgen = false;

    auto sampler = [&](vec3 pos) -> BlockType {return GetBlock(pos);};

    for (int i=0; i<render_distance*2+1; i++) {
        for (int j=0; j<render_distance*2+1; j++) {
            if (tryMeshBuild[i][j] == false) continue;
            chunks[i][j].GenerateMesh(sampler);
            tryMeshBuild[i][j] = false;
            hasgen = true;
            break;
        }
        if (hasgen) break;
    }
    return hasgen;
}

void ChunkManager::Update(vec3 player_pos) {
    vec2 player_chunk = GetChunk(player_pos);
    vec2 diff = player_chunk - last_player_position;
    int diffi = diff.x;
    int diffj = diff.y;

    if (diffi || diffj)  {
        last_player_position = player_chunk;
        ShiftMap(diffi,diffj);
    }

    bool hasgen = TryGen();
    if (!hasgen) TryMesh();
}

// O(n^2) translucency with a bfs
void ChunkManager::Draw() {
    struct vec2i {
        int i,j;
    };

    std::queue<vec2i> q;
    std::vector<std::vector<bool>> mask(render_distance*2+1,std::vector<bool>(render_distance*2+1,false));

    int di[] = {1,-1,0,0};
    int dj[] = {0,0,1,-1};

    q.push({0,0});
    q.push({0,render_distance*2});
    q.push({render_distance*2,0});
    q.push({render_distance*2,render_distance*2});

    mask[render_distance*2][render_distance*2] = 1;
    mask[render_distance*2][0] = 1;
    mask[0][render_distance*2] = 1;
    mask[0][0] = 1;

    while (!q.empty()) {
        int ti = q.front().i,tj = q.front().j;
        if (isGenerated[ti][tj]) chunks[ti][tj].Draw();
        for (int f=0; f<4; f++) {
            int i = ti + di[f],j = tj + dj[f];
            if (i < 0 || j < 0 || i > render_distance*2 || j > render_distance*2) continue;
            if (mask[i][j]) continue;
            mask[i][j] = 1;
            q.push({i,j});
        }
        q.pop();
    }
}

void ChunkManager::Clean() {
    for (int i=0; i<render_distance*2 + 1; i++) {
        for (int j=0; j<render_distance*2 + 1; j++) {
            if (!isGenerated[i][j]) continue;
            chunks[i][j].Clean();
        }
    }
}
