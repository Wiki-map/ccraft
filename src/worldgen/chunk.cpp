#include "worldgen/chunk.h"

#define CHUNK_MAX_TRIANGLE CHUNK_SIZE * CHUNK_SIZE * 12 + 16

Chunk::Chunk(vec2 pos) {
    position = pos;
    mesh = Mesh(CHUNK_MAX_TRIANGLE);
}

void Chunk::Generate(std::function<float(float,float)> noise_fun) {

    mesh.Clear();

    for (int i=0; i<CHUNK_SIZE; i++) {
        for (int j=0; j<CHUNK_SIZE; j++) {
            vec3 top_pos = {(float)i + position.x, 0, (float)j + position.y};
            top_pos.y = (int)(noise_fun(top_pos.x,top_pos.z));

            Color color = rgba(0, 237, 135, 255);
            if (top_pos.y > 8 - rand()%3) color = rgba(184, 184, 184, 255);
            if (top_pos.y > 20 - rand()%3) color = rgba(240,240,240,255);
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
            vec3 normt = {0,1,0};
            mesh.PushTriangle(
                top_pos,top_top_right,top_bot_left,color,normt,1
            );
            mesh.PushTriangle(
                top_bot_left,top_top_right,top_bot_right,color,normt,1
            );


            //bot face
            vec3 normb = {0,-1,0};
            mesh.PushTriangle(
                bot_pos,bot_top_right,bot_bot_left,color,normb,1
            );
            mesh.PushTriangle(
                bot_bot_left,bot_top_right,bot_bot_right,color,normb,1
            );


            //right face
            vec3 normr = {1,0,0};
            mesh.PushTriangle(
                top_top_right,top_bot_right,bot_top_right,color,normr,1
            );
            mesh.PushTriangle(
                bot_bot_right,top_bot_right,bot_top_right,color,normr,1
            );


            //left face
            vec3 norml = {-1,0,0};
            mesh.PushTriangle(
                top_pos,top_bot_left,bot_pos,color,norml,1
            );
            mesh.PushTriangle(
                bot_bot_left,top_bot_left,bot_pos,color,norml,1
            );


            //front face
            vec3 normf = {0,0,-1};
            mesh.PushTriangle(
                top_bot_left,top_bot_right,bot_bot_left,color,normf,1
            );
            mesh.PushTriangle(
                bot_bot_right,top_bot_right,bot_bot_left,color,normf,1
            );


            //back face
            vec3 normbk = {0,0,1};
            mesh.PushTriangle(
                top_pos,top_top_right,bot_pos,color,normbk,1
            );
            mesh.PushTriangle(
                bot_top_right,top_top_right,bot_pos,color,normbk,1
            );
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
