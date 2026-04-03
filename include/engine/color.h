#pragma once

#include "utils.h"
#include <cstdint>

struct Color {
    uint8_t r,g,b,a;
};

vec4 frgba(uint8_t r,uint8_t g,uint8_t b,uint8_t a);
vec4 frgba(Color color);
vec4 frgba(float r,float g,float b,float a);

Color rgba(uint8_t r,uint8_t g,uint8_t b,uint8_t a);

void ClearBackground(Color c);
