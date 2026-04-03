#include "worldgen/chunk.h"

#define CHUNK_MAX_TRIANGLE CHUNK_SIZE * CHUNK_SIZE * 12 * 4 + 16

Chunk::Chunk(vec2 pos) {
    position = pos;
    mesh = Mesh(CHUNK_MAX_TRIANGLE);
}

void Chunk::SpawnBlock(vec3 pos,std::function<int32_t(float,float)> height_sampler) {
    vec3 top_pos = pos;

    float i = pos.x;
    float j = pos.z;

    Color color = rgba(0, 237, 135, 255);
    if (top_pos.y > 8 - rand()%3) color = rgba(184, 184, 184, 255);
    if (top_pos.y > 20 - rand()%3) color = rgba(240,240,240,255);
    if (top_pos.y == 1  || top_pos.y == 0) {
        color = rgba(240, 205, 108,255);
    }
    if (top_pos.y < 0) {
        top_pos.y = 0;
        color = rgba(47, 114, 222,255);
    }

    vec3 top_top_right = top_pos; top_top_right.x ++;
    vec3 top_bot_left = top_pos; top_bot_left.z --;
    vec3 top_bot_right = top_pos; top_bot_right.x ++; top_bot_right.z --;

    vec3 bot_pos = top_pos; bot_pos.y --;
    vec3 bot_top_right = bot_pos; bot_top_right.x++;
    vec3 bot_bot_left = bot_pos; bot_bot_left.z --;
    vec3 bot_bot_right = bot_pos; bot_bot_right.x ++; bot_bot_right.z --;

    // 12 triangles

    //top face
    if (pos.y == (float)height_sampler(i,j)) {
        vec3 normt = {0,1,0};
        mesh.PushTriangle(
            top_pos,top_top_right,top_bot_left,color,normt,1
        );
        mesh.PushTriangle(
            top_bot_left,top_top_right,top_bot_right,color,normt,1
        );
    }

    //bot face
    //vec3 normb = {0,-1,0};
    //mesh.PushTriangle(
    //    bot_pos,bot_top_right,bot_bot_left,color,normb,1
    //);
    //mesh.PushTriangle(
    //    bot_bot_left,bot_top_right,bot_bot_right,color,normb,1
    //);


    //right face
    if (height_sampler(i+1,j) < pos.y) {
        vec3 normr = {1,0,0};
        mesh.PushTriangle(
            top_top_right,top_bot_right,bot_top_right,color,normr,1
        );
        mesh.PushTriangle(
            bot_bot_right,top_bot_right,bot_top_right,color,normr,1
        );
    }

    //left face
    if (height_sampler(i-1,j) < pos.y) {
        vec3 norml = {-1,0,0};
        mesh.PushTriangle(
            top_pos,top_bot_left,bot_pos,color,norml,1
        );
        mesh.PushTriangle(
            bot_bot_left,top_bot_left,bot_pos,color,norml,1
        );
    }

    //front face
    if (height_sampler(i,j-1) < pos.y) {
        vec3 normf = {0,0,-1};
        mesh.PushTriangle(
            top_bot_left,top_bot_right,bot_bot_left,color,normf,1
        );
        mesh.PushTriangle(
            bot_bot_right,top_bot_right,bot_bot_left,color,normf,1
        );
    }

    //back face
    if (height_sampler(i,j+1) < pos.y) {
        vec3 normbk = {0,0,1};
        mesh.PushTriangle(
            top_pos,top_top_right,bot_pos,color,normbk,1
        );
        mesh.PushTriangle(
            bot_top_right,top_top_right,bot_pos,color,normbk,1
        );
    }
}

void Chunk::GenerateHeight(std::function<float(float,float)> noise_fun) {
    for (int i=0; i<CHUNK_SIZE; i++) {
        for (int j=0; j<CHUNK_SIZE; j++) {
            height[i][j] = (int)noise_fun((float)i + position.x,(float)j + position.y);
            if (height[i][j] > 255) height[i][j] = 255;
            if (height[i][j] < 0) height[i][j] = -1;
        }
    }
}

void Chunk::GenerateMesh(std::function<int32_t(float,float)> height_sampler) {

    mesh.Clear();

    for (int i=0; i<CHUNK_SIZE; i++) {
        for (int j=0; j<CHUNK_SIZE; j++) {
            vec3 pos = {(float)i + position.x,(float) height[i][j],(float)j + position.y};
            int aux = 280;

            float fi = i + position.x;
            float fj = j + position.y;

            aux = std::min(aux,height_sampler(fi-1,fj));
            aux = std::min(aux,height_sampler(fi+1,fj));
            aux = std::min(aux,height_sampler(fi,fj-1));
            aux = std::min(aux,height_sampler(fi,fj+1));

            aux = height[i][j] - aux;
            if (aux < 1) aux = 1;

            for (int k=1; k<=aux; k++) {
                SpawnBlock(pos,height_sampler);
                pos.y --;
            }
        }
    }

    mesh.UpdeteMesh();
}

void Chunk::Draw() {
    mesh.Draw();
}

void Chunk::Clean() {
    mesh.Clean();
}

#include <iostream>

int32_t Chunk::GetHeight(float i,float j) {
    int di = i - position.x;
    int dj = j - position.y;

    if (di < 0 || dj < 0 || di >= CHUNK_SIZE || dj >= CHUNK_SIZE) {
        return -10;
    }

    return height[di][dj];
}
