#pragma once

#include "engine/texture.h"
#include "engine/shader.h"
#include "engine/mesh.h"

struct Color {
    uint8_t r,g,b,a;
};

void UseShader(Shader shader);
void DrawMesh(const Mesh &m);

void ClearBackground(Color c);