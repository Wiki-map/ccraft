#include "engine/renderer.h"

#include "glad/gl.h"

void UseShader(Shader shader) {
    glUseProgram(shader.GetShaderID());
}

void ClearBackground(Color c) {
    glClearColor((float)c.r/255.0f,(float)c.g/255.0f, (float)c.b/255.0f, (float)c.a/255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}