#include "engine/color.h"

#include "glad/gl.h"

vec4 frgba(uint8_t r,uint8_t g,uint8_t b,uint8_t a) {
    return {(float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/255.0f};
}

vec4 frgba(Color color) {
    return {(float)color.r/255.0f,(float)color.g/255.0f,(float)color.b/255.0f,(float)color.a/255.0f};
}

vec4 frgba(float r,float g,float b,float a) {
    return {r,g,b,a};
}

Color rgba(uint8_t r,uint8_t g,uint8_t b,uint8_t a) {
    return {r,g,b,a};
}

uint32_t rgba32(uint8_t r,uint8_t g,uint8_t b,uint8_t a) {
    return rgba32({r,g,b,a});
}
uint32_t rgba32(Color color) {
    void * val = &color;
    uint32_t *p = (uint32_t *)val;
    return *p;
}


void ClearBackground(Color c) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor((float)c.r/255.0f,(float)c.g/255.0f, (float)c.b/255.0f, (float)c.a/255.0f);
}
