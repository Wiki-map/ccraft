#include <iostream>
#include "engine/window.h"
#include "engine/renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"
#include "player.h"

int main(){
    InitWindow(800,800,"gl");

    Texture tex = LoadTexture(RESOURCE_PATH "cobble.png");
    Shader s = Shader(RESOURCE_PATH "vertex.glsl", RESOURCE_PATH "fragment.glsl");

    int32_t u_tex[8] = {0,1,2,3,4,5,6,7};
    s.SetIntArrayUniform("u_tex",u_tex,8);

    float pos[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    vec2 uv1 = {0,1};
    vec2 uv2 = {1,1};
    vec2 uv3 = {0,0};
    vec2 uv4 = {1,0};

    Mesh mesh = Mesh(512);

    mesh.Clear();
    
    for (int i=1; i<=10; i++) {
        vec3 npos = {rand()%10,rand()%10,rand()%10};
        for (int i=0; i<6; i++) {
            vec3 p1 = {pos[i*6*3],pos[i*6*3+1],pos[i*6*3+2]};
            vec3 p2 = {pos[(i*6+1)*3],pos[(i*6+1)*3+1],pos[(i*6+1)*3+2]};
            vec3 p3 = {pos[(i*6+2)*3],pos[(i*6+2)*3+1],pos[(i*6+2)*3+2]};

            p1.x += npos.x; p1.y += npos.y; p1.z += npos.z;
            p2.x += npos.x; p2.y += npos.y; p2.z += npos.z;
            p3.x += npos.x; p3.y += npos.y; p3.z += npos.z;

            mesh.PushTriangle(p1,p2,p3,uv1,uv2,uv3,tex);

            vec3 p4 = {pos[(i*6+3)*3],pos[(i*6+3)*3+1],pos[(i*6+3)*3+2]};
            vec3 p5 = {pos[(i*6+4)*3],pos[(i*6+4)*3+1],pos[(i*6+4)*3+2]};
            vec3 p6 = {pos[(i*6+5)*3],pos[(i*6+5)*3+1],pos[(i*6+5)*3+2]};

            p4.x += npos.x; p4.y += npos.y; p4.z += npos.z;
            p5.x += npos.x; p5.y += npos.y; p5.z += npos.z;
            p6.x += npos.x; p6.y += npos.y; p6.z += npos.z;

            mesh.PushTriangle(p4,p5,p6,uv3,uv4,uv1,tex);
        }
    }

    mesh.UpdeteMesh();

    Player player = Player({0,0,0});

    while (!IsWindowClosed()) {
        ClearBackground(SKY_BLUE);

        if (IsKeyDown(KeyboardKey::ESCAPE)) {
            break;
        }

        player.Update();

        UseCamera(s,player.GetCamera());

        UseShader(s);
        mesh.Draw();
    }

    DeleteWindow();
}
