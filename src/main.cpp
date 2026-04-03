#include <iostream>
#include "glad/gl.h"
#include "engine/window.h"
#include "engine/color.h"
#include "worldgen/chunk.h"
#include "camera.h"
#include "player.h"
#include "worldgen/extern/FastNoise.h"

const int32_t map_len = 64;
const int32_t chunk_count = map_len*map_len;
Chunk chunks[chunk_count + 5];

int main(){
    InitWindow(800,800,"gl");

    Shader s = Shader(RESOURCE_PATH "vertex.glsl", RESOURCE_PATH "fragment.glsl");

    s.SetVector3Uniform("u_light_pos", glm::vec3(0,1000,0));

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j] = Chunk({(float)(i - map_len/2)*CHUNK_SIZE,(float)(j - map_len/2)*CHUNK_SIZE});
        }
    }

    FastNoise n,n2;
    n.SetNoiseType(FastNoise::NoiseType::Perlin);
    n2.SetNoiseType(FastNoise::NoiseType::PerlinFractal);

    n.SetFrequency(0.01);

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j].Generate([&](float x,float y)-> float {
                return n.GetPerlin(x,y)*30 + n2.GetPerlinFractal(x,y)*30;
            });
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
