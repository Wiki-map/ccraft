#include "worldgen/chunk.h"
#include "engine/mesh.h"
#include <array>
#include <functional>

Chunk::Chunk(vec2 pos) {
    position = pos;
    const int padding = 10;
    voxels.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT + padding);
}

void Chunk::Init() {
    const int max_block_per_position = 2;
    const int triangle_per_cube = 12;
    mesh.Init();
    translucent.Init();
}

void Chunk::GenerateVoxels(std::function<float(float,float)> noise_function) {
    std::array<std::array<int,CHUNK_SIZE>,CHUNK_SIZE> heights;

    for (float i=0; i<CHUNK_SIZE; i++) {
        for (float j=0; j<CHUNK_SIZE; j++) {
            heights[i][j] = noise_function(position.x + i*BLOCK_SIZE,position.y + j*BLOCK_SIZE);
        }
    }

    for (float x=0; x<CHUNK_SIZE; x++) {
        for (float z=0; z<CHUNK_SIZE; z++) {
            for (float y=0; y<CHUNK_HEIGHT; y++) {
                if (y > heights[x][z]) break;
                int32_t idx = x*CHUNK_SIZE * CHUNK_HEIGHT + y * CHUNK_SIZE + z;
                if (y > heights[x][z]) voxels[idx] = BlockType::AIR;
                else if (y == heights[x][z] && y > WATTER_LEVEL) voxels[idx] = BlockType::GRASS;
                else if (y == heights[x][z]) voxels[idx] = BlockType::SAND;
                else if (heights[x][z] - y <= 3) {
                    voxels[idx] = BlockType::DIRT;
                }
                else {
                    voxels[idx] = BlockType::STONE;
                }
            }
        }
    }
}

BlockType Chunk::GetBlock(vec3 pos) {
    if (pos.x < position.x || pos.x >= position.x + CHUNK_SIZE * BLOCK_SIZE ||
        pos.z < position.y || pos.z >= position.y + CHUNK_SIZE * BLOCK_SIZE ||
        pos.y < 0 || pos.y >= CHUNK_HEIGHT) {
        return BlockType::AIR;
    }
    vec3 rel = pos - (vec3){position.x,0,position.y};
    vec3 posidx = rel / BLOCK_SIZE;
    int32_t idx = posidx.x * CHUNK_SIZE * CHUNK_HEIGHT + posidx.y * CHUNK_SIZE + posidx.z;
    //std::cout<<(int)voxels[idx]<<"\n";
    return voxels[idx];
}

Color GetBlockColor(BlockType type) {
    if (type == BlockType::DIRT) return rgba(97, 68, 9,255);
    if (type == BlockType::GRASS) return rgba(28, 184, 95,255);
    if (type == BlockType::STONE) return rgba(145, 145, 145,255);
    if (type == BlockType::SAND) return rgba(247, 204, 84,255);
    return rgba(0,0,0,0);
}

void Chunk::PushCube(vec3 top_pos,std::function<BlockType(vec3)> voxel_sampler) {
    vec3 top_top_right = top_pos; top_top_right.x += BLOCK_SIZE;
    vec3 top_bot_left = top_pos; top_bot_left.z += BLOCK_SIZE;
    vec3 top_bot_right = top_bot_left; top_bot_right.x += BLOCK_SIZE;

    vec3 bot_pos = top_pos; bot_pos.y -= BLOCK_SIZE;
    vec3 bot_top_right = bot_pos; bot_top_right.x += BLOCK_SIZE;
    vec3 bot_bot_left = bot_pos; bot_bot_left.z += BLOCK_SIZE;
    vec3 bot_bot_right = bot_bot_left; bot_bot_right.x += BLOCK_SIZE;

    Color color = GetBlockColor(voxel_sampler(top_pos));

    if (top_pos.y < WATTER_LEVEL*BLOCK_SIZE && voxel_sampler(top_pos + (vec3){0,BLOCK_SIZE,0}) == BlockType::AIR) {
        vec3 norm = {0,1,0};
        Color colorw = rgba(44, 110, 232,150);
        vec3 topw = top_pos; topw.y = WATTER_LEVEL*BLOCK_SIZE;
        vec3 top_top_rightw = topw; top_top_rightw.x += BLOCK_SIZE;
        vec3 top_bot_leftw = topw; top_bot_leftw.z += BLOCK_SIZE;
        vec3 top_bot_rightw = top_bot_leftw; top_bot_rightw.x += BLOCK_SIZE;
        translucent.PushQuad(topw,top_top_rightw,top_bot_leftw,top_bot_rightw,colorw,NormalDir::UP);
    }

    if (voxel_sampler(top_pos + (vec3){0,BLOCK_SIZE,0}) == BlockType::AIR) {
        mesh.PushQuad(top_pos,top_top_right,top_bot_left,top_bot_right,color,NormalDir::UP);
    }

    if (voxel_sampler(top_pos + (vec3){0,-BLOCK_SIZE,0}) == BlockType::AIR) {
        mesh.PushQuad(bot_bot_left,bot_bot_right,bot_pos,bot_top_right,color,NormalDir::DOWN);
    }


    //left face
    if (voxel_sampler(top_pos + (vec3){-BLOCK_SIZE,0,0}) == BlockType::AIR) { 
        mesh.PushQuad(top_pos,top_bot_left,bot_pos,bot_bot_left,color,NormalDir::LEFT);
    }

    if (voxel_sampler(top_pos + (vec3){BLOCK_SIZE,0,0}) == BlockType::AIR) {
        mesh.PushQuad(top_bot_right,top_top_right,bot_bot_right,bot_top_right,color,NormalDir::RIGHT);
    }

    //front face
    if (voxel_sampler(top_pos + (vec3){0,0,BLOCK_SIZE}) == BlockType::AIR) {
        mesh.PushQuad(top_bot_left,top_bot_right,bot_bot_left,bot_bot_right,color,NormalDir::FRONT);
    }
    if (voxel_sampler(top_pos + (vec3){0,0,-BLOCK_SIZE}) == BlockType::AIR) {
        mesh.PushQuad(top_top_right,top_pos,bot_top_right,bot_pos,color,NormalDir::BACK);
    }

}

void Chunk::GenerateMesh(std::function<BlockType(vec3)> voxel_sampler) {
    mesh.Clear();
    translucent.Clear();
    for (float x=0; x<CHUNK_SIZE; x++) {
        for (float y=0; y<CHUNK_HEIGHT; y++) {
            for (float z=0; z<CHUNK_SIZE; z++) {
                if (voxels[x*CHUNK_SIZE*CHUNK_HEIGHT + y*CHUNK_SIZE + z] == BlockType::AIR) continue;
                vec3 pos = {position.x + x*BLOCK_SIZE,y*BLOCK_SIZE, position.y + z*BLOCK_SIZE};
                this->PushCube(pos,voxel_sampler);
            }
        }
    }
    mesh.UpdeteMesh();
    translucent.UpdeteMesh();
}

void Chunk::Draw() {
    mesh.Draw();
    translucent.Draw();
}

void Chunk::Clean() {
    mesh.Clean();
    translucent.Clean();
}

vec2 Chunk::GetPosition() {return position;}
