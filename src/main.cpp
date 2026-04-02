#include <iostream>
#include "glad/gl.h"
#include "engine/window.h"
#include "engine/renderer.h"
#include "engine/texture.h"
#include "worldgen/chunk.h"
#include "camera.h"
#include "player.h"

const int32_t map_len = 64;
const int32_t chunk_count = map_len*map_len;
Chunk chunks[chunk_count + 5];

int main(){
    InitWindow(800,800,"gl");

    Texture tex = LoadTexture(RESOURCE_PATH "cobble.png");
    Shader s = Shader(RESOURCE_PATH "vertex.glsl", RESOURCE_PATH "fragment.glsl",8);

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j] = Chunk({(float)(i - map_len/2)*CHUNK_SIZE,(float)(j - map_len/2)*CHUNK_SIZE});
        }
    }

    FastNoise n;
    n.SetNoiseType(FastNoise::NoiseType::Perlin);

    for (int i=0; i<map_len; i++) {
        for (int j=0; j<map_len; j++) {
            chunks[i*map_len + j].Generate(n,tex);
        }
    }


    Player player = Player({0,0,0});

    while (!IsWindowClosed()) {
        ClearBackground(SKY_BLUE);

        std::cout<<"[INFO]: FPS is: "<<1.0f/GetDeltaTime()<<"\n";


        if (IsKeyDown(KeyboardKey::ESCAPE)) {
            break;
        }

        player.Update(GetDeltaTime());

        UseCamera(s,player.GetCamera());

        UseShader(s);

        for (int i=0; i<map_len; i++) {
            for (int j=0; j<map_len; j++) {
                chunks[i*map_len + j].Draw();
            }
        }
    }

    DeleteWindow();
}
