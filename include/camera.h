#pragma once

#include "utils.h"
#include "engine/shader.h"

struct Camera {
    vec3 position;
    vec3 target;
    float fov,near,far;
};

void UseCamera(Shader shader,Camera camera);
