#include <iostream>
#include "glad/gl.h"
#include "engine/window.h"
#include "engine/color.h"
#include "worldgen/chunk.h"
#include "camera.h"
#include "player.h"
#include "worldgen/extern/FastNoise.h"
#include <map>

const int32_t map_len = 1280 / CHUNK_SIZE;
const int32_t chunk_count = map_len*map_len;
Chunk chunks[chunk_count + 5];
std::map<std::pair<int,int>,int> midx;

int main(){
    InitWindow(800,800,"gl");

    Shader s = Shader(RESOURCE_PATH "vertex.glsl", RESOURCE_PATH "fragment.glsl");

    s.SetVector3Uniform("u_light_pos", glm::vec3(0,map_len * CHUNK_SIZE * 2,0));

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j] = Chunk({(float)(i - map_len/2)*CHUNK_SIZE,(float)(j - map_len/2)*CHUNK_SIZE});
            midx[{i - map_len/2,j - map_len/2}] = i*map_len + j;
        }
    }

    FastNoise n,n2,n3;

    n.SetSeed(time(0));
    n2.SetSeed(time(0));
    n3.SetSeed(time(0));

    n.SetNoiseType(FastNoise::NoiseType::Perlin);
    n2.SetNoiseType(FastNoise::NoiseType::PerlinFractal);
    n3.SetNoiseType(FastNoise::NoiseType::Perlin);

    n.SetFrequency(0.01);
    n2.SetFrequency(0.03);
    n3.SetFrequency(0.1);

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j].GenerateHeight([&](float x,float y)-> float {
                return n.GetPerlin(x,y) * n2.GetPerlinFractal(x,y) * 100 + n.GetPerlin(x,y)*30 + n3.GetPerlin(x,y)*3;
            });
        }
    }

    auto height_smap = [&](float i,float j) -> int32_t {
        int32_t chi,chj;

        if (i >= 0) chi = i / CHUNK_SIZE;
        if (j >= 0) chj = j / CHUNK_SIZE;

        if (i < 0) chi = (i - CHUNK_SIZE + 1) / CHUNK_SIZE;
        if (j < 0) chj = (j - CHUNK_SIZE + 1) / CHUNK_SIZE;

        //std::cout<<chi<<" "<<chj<<"\n";

        int idx = midx[{chi,chj}];
        if (idx < 0 || idx >= chunk_count) {
            return 0;
        }

        return chunks[idx].GetHeight(i,j);
    };

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j].GenerateMesh(height_smap);
        }
    }


    Player player = Player({0,0,0});

    while (!IsWindowClosed()) {
        ClearBackground(rgba(130,200,229,255));

        std::cout<<"[INFO]: FPS is: "<<1.0f/GetDeltaTime()<<"\n";


        if (IsKeyDown(KeyboardKey::ESCAPE)) {
            break;
        }

        player.Update(GetDeltaTime());

        UseCamera(s,player.GetCamera());

        s.Use();

        for (int i=0; i<map_len; i++) {
            for (int j=0; j<map_len; j++) {
                chunks[i*map_len + j].Draw();
            }
        }
    }

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j].Clean();
        }
    }


    DeleteWindow();
}
