#include "worldgen/chunk_manager.h"
#include "worldgen/chunk.h"
#include "worldgen/extern/FastNoise.h"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <utility>

ChunkManager::ChunkManager(int render_distance) {
    this->render_distance = render_distance;
    postion = {-render_distance*CHUNK_SIZE*BLOCK_SIZE,-render_distance*CHUNK_SIZE*BLOCK_SIZE};
    last_player_position = {0,0};
    chunks = std::vector<std::vector<Chunk>>(render_distance*2 + 1,std::vector<Chunk>(render_distance*2 + 1));
    isGenerated = std::vector<std::vector<bool>>(render_distance*2 + 1,std::vector<bool>(render_distance*2 + 1,false));
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

void ChunkManager::GenChunk(int i,int j,std::function<BlockType(vec3)> sampler) {
    chunks[i][j].GenerateMesh(sampler);
}

void ChunkManager::Update(vec3 player_pos) {
    vec2 player_chunk = GetChunk(player_pos);
    vec2 diff = player_chunk - last_player_position;
    int diffi = diff.x;
    int diffj = diff.y;

    if (diffi || diffj) last_player_position = player_chunk;

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
                if (j < diffj) {
                    isGenerated[i][j] = false;
                }
            }
        }
    }

    FastNoise n;
    n.SetSeed(0);
    n.SetNoiseType(FastNoise::NoiseType::CubicFractal);
    auto noise = [&](float x,float y) -> float {
        return n.GetCubicFractal(x,y)*30 + 30;
    };

    auto sampler = [&](vec3 pos) -> BlockType {return GetBlock(pos);};

    bool hasgen = false;
    for (int i=0; i<render_distance*2 + 1; i++) {
        for (int j=0; j<render_distance*2 + 1; j++) {
            if (isGenerated[i][j]) continue;
            //if ((i-render_distance)*(i-render_distance) + (j-render_distance)*(j-render_distance) > render_distance*render_distance) continue;

            float x = postion.x + (render_distance*2 - i)*CHUNK_SIZE*BLOCK_SIZE;
            float y = postion.y + j*CHUNK_SIZE*BLOCK_SIZE;
            chunks[i][j] = Chunk({x,y});
            chunks[i][j].Init();
            chunks[i][j].GenerateVoxels(noise);
            isGenerated[i][j] = true;

            GenChunk(i,j,sampler);
            if (i > 0 && isGenerated[i-1][j]) GenChunk(i-1,j,sampler);
            if (j > 0 && isGenerated[i][j-1]) GenChunk(i,j-1,sampler);
            if (i < render_distance*2 && isGenerated[i+1][j]) GenChunk(i+1,j,sampler);
            if (j < render_distance*2 && isGenerated[i][j+1]) GenChunk(i,j+1,sampler);


            hasgen = true;
            break;
        }
        if (hasgen) break;
    }
}

void ChunkManager::Draw() {

    struct val {
        int i,j;
    };

    std::vector<val> c;

    for (int i=0; i<render_distance*2 + 1; i++) {
        for (int j=0; j<render_distance*2 + 1; j++) {
            if (!isGenerated[i][j]) continue;
            c.push_back({i,j});
        }
    }

    std::sort(c.begin(),c.end(),[&](val a,val b) -> bool {
        float adi = std::abs(a.i - render_distance);
        float adj = std::abs(a.j - render_distance);
        float bdi = std::abs(b.i - render_distance);
        float bdj = std::abs(b.j - render_distance);

        return adi*adi + adj*adj > bdi*bdi + bdj*bdj;
    });

    for (auto [i,j]: c) {
        chunks[i][j].Draw();
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
