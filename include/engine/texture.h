#pragma once

#include <cstdint>
#include <string>

struct Texture {
    uint32_t id;
    int16_t width,height;
};

Texture LoadTexture(std::string path);