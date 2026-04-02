#include "worldgen/chunk.h"

#define CHUNK_MAX_TRIANGLE CHUNK_SIZE * CHUNK_SIZE * 12 + 16

Chunk::Chunk(vec2 pos) {
    position = pos;
    chunk_mesh = Mesh(CHUNK_MAX_TRIANGLE);
}

void Chunk::Generate(FastNoise noise_maker,Texture tex) {

    chunk_mesh.Clear();

    for (int i=0; i<CHUNK_SIZE; i++) {
        for (int j=0; j<CHUNK_SIZE; j++) {
            vec3 top_pos = {(float)i + position.x, 0, (float)j + position.y};
            top_pos.y = (int)(noise_maker.GetPerlin(top_pos.x,top_pos.z) * 30);

            vec2 uv1 = {0,0}; // top_bot_right | 
            vec2 uv2 = {0,1}; // top_pos |
            vec2 uv3 = {1,0}; // top_bot_left |
            vec2 uv4 = {1,1}; // top_top_right |

            vec3 top_top_right = top_pos; top_top_right.x ++;
            vec3 top_bot_left = top_pos; top_bot_left.z --;
            vec3 top_bot_right = top_pos; top_bot_right.x ++; top_bot_right.z --;

            vec3 bot_pos = top_pos; bot_pos.y --;
            vec3 bot_top_right = bot_pos; bot_top_right.x++;
            vec3 bot_bot_left = bot_pos; bot_bot_left.z --;
            vec3 bot_bot_right = bot_pos; bot_bot_right.x ++; bot_bot_right.z --;

            // 12 triangles

            //top face
            vec3 normt = {0,1,0};
            chunk_mesh.PushTriangle(
                top_pos,top_top_right,top_bot_left,uv2,uv4,uv3,tex,normt,1
            );
            chunk_mesh.PushTriangle(
                top_bot_left,top_top_right,top_bot_right,uv3,uv4,uv1,tex,normt,1
            );


            //bot face
            vec3 normb = {0,-1,0};
            chunk_mesh.PushTriangle(
                bot_pos,bot_top_right,bot_bot_left,uv1,uv2,uv3,tex,normb,1
            );
            chunk_mesh.PushTriangle(
                bot_bot_left,bot_top_right,bot_bot_right,uv1,uv2,uv3,tex,normb,1
            );


            //right face
            vec3 normr = {1,0,0};
            chunk_mesh.PushTriangle(
                top_top_right,top_bot_right,bot_top_right,uv1,uv2,uv3,tex,normr,1
            );
            chunk_mesh.PushTriangle(
                bot_bot_right,top_bot_right,bot_top_right,uv1,uv2,uv3,tex,normr,1
            );


            //left face
            vec3 norml = {-1,0,0};
            chunk_mesh.PushTriangle(
                top_pos,top_bot_left,bot_pos,uv1,uv2,uv3,tex,norml,1
            );
            chunk_mesh.PushTriangle(
                bot_bot_left,top_bot_left,bot_pos,uv1,uv2,uv3,tex,norml,1
            );


            //front face
            vec3 normf = {0,0,-1};
            chunk_mesh.PushTriangle(
                top_bot_left,top_bot_right,bot_bot_left,uv1,uv2,uv3,tex,normf,1
            );
            chunk_mesh.PushTriangle(
                bot_bot_right,top_bot_right,bot_bot_left,uv1,uv2,uv3,tex,normf,1
            );

            //back face
            vec3 normbk = {0,0,1};
            chunk_mesh.PushTriangle(
                top_pos,top_top_right,bot_pos,uv1,uv2,uv3,tex,normbk,1
            );
            chunk_mesh.PushTriangle(
                bot_top_right,top_top_right,bot_pos,uv1,uv2,uv3,tex,normbk,1
            );


        }
    }

    chunk_mesh.UpdeteMesh();
}

void Chunk::Draw() {
    chunk_mesh.Draw();
}
