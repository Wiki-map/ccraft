#pragma once

#include "engine/texture.h"
#include "engine/shader.h"
#include "engine/mesh.h"

struct Color {
    uint8_t r,g,b,a;
};

void UseShader(Shader shader);

void ClearBackground(Color c);

#define BLACK {0,0,0,255}
#define WHITE {255,255,255,255}
#define CLEAR {0,0,0,0}
#define SKY_BLUE {130,200,229,255}