#include "imgui.h"
#include "glad/gl.h"
#include "engine/window.h"
#include "engine/color.h"
#include "camera.h"
#include "player.h"
#include "worldgen/chunk_manager.h"
#include "worldgen/extern/FastNoiseLite.h"

int main(){
    InitWindow(800,800,"gl");

    Shader s = Shader(RESOURCE_PATH "vertex.glsl", RESOURCE_PATH "fragment.glsl");
    s.SetVector3Uniform("u_light_pos", glm::vec3(0,1e9,0));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ChunkManager chunk_manager(24);

    FastNoiseLite n;
    n.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    //n.SetFractalType(FastNoiseLite::FractalType_PingPong);
    n.SetFrequency(0.01);

    auto getnoise = [&](float a,float b) -> float {
        return n.GetNoise(a,b)*10 + 28;
    };

    chunk_manager.SetNoiseFuction(getnoise);

    Player player = Player({0,40,0});

    bool cr = true;
    EnableCursor();

    while (!IsWindowClosed()) {
        ClearBackground(rgba(130,200,229,255));

        if (IsKeyDown(KeyboardKey::ESCAPE)) {
            break;
        }

        if (IsKeyPresed(KeyboardKey::C)) {
            if (cr) DisableCursor();
            else EnableCursor();
            cr ^= 1;
        }

        player.Update(GetDeltaTime());

        chunk_manager.Update(player.GetPosition());

        {
            ImGui::Begin("info");
            ImGui::Text("fps is: %f",1.0f/GetDeltaTime());
            ImGui::Text("player position is: [%f,%f,%f]",player.GetPosition().x,player.GetPosition().y,player.GetPosition().z);
            vec2 x = chunk_manager.GetChunk(player.GetPosition());
            ImGui::Text("player position is: [%f,%f]",x.x,x.y);
            ImGui::End();

            ImGui::Begin("debug");
            bool wf;
            ImGui::Checkbox("wireframe", &wf);
            ImGui::End();

            if (wf) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        }

        UseCamera(s,player.GetCamera());
        s.Use();
        chunk_manager.Draw();

        ImGuiDrawOpengl();
    }

    DeleteWindow();
}
