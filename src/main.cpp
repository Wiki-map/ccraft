#include <iostream>
#include "engine/window.h"
#include "engine/renderer.h"

int main(){
    InitWindow(800,800,"gl");

    Texture tex = LoadTexture(RESOURCE_PATH "cobble.png");
    Shader s = Shader(RESOURCE_PATH "vertex.glsl", RESOURCE_PATH "fragment.glsl");

    int32_t u_tex[8] = {0,1,2,3,4,5,6,7};
    s.SetIntArrayUniform("u_tex",u_tex,8);

    vec3 p1 = {0.5,0.5,0.5};
    vec3 p2 = {-0.5,0.5,0.5};
    vec3 p3 = {-0.5,-0.5,0.5};
    vec3 p4 = {0.5,-0.5,0.5};

    vec2 uv1 = {0,0};
    vec2 uv2 = {1,0};
    vec2 uv3 = {0,1};
    vec2 uv4 = {1,1};

    Mesh mesh = Mesh(512);

    mesh.Clear();
    mesh.PushTriangle(
        p1,p2,p3,uv1,uv3,uv2,tex
    );
    mesh.PushTriangle(
        p1,p3,p4,uv1,uv3,uv4,tex
    );
    mesh.UpdeteMesh();

    while (!IsWindowClosed()) {
        ClearBackground({255,255,255,255});

        UseShader(s);
        mesh.Draw();
    }

    DeleteWindow();
}
