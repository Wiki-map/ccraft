#include "engine/texture.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/gl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "engine/extern/stb_image.h"

static std::string GetFileContend(std::string path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Texture LoadTexture(std::string path) {
    Texture rez;
    glGenTextures(1,&rez.id);
    glBindTexture(GL_TEXTURE_2D, rez.id);

    int width,height,nrChannels;
    unsigned char* data = stbi_load(path.c_str(),&width,&height,&nrChannels,0);

    rez.width = width;
    rez.height = height;

    if (!data) {
        std::cerr<<"[ERROR]: Failed to load texture with id: "<< rez.id << "\n";
        exit(-1);
    }

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return rez;
}