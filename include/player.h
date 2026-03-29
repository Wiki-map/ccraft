#pragma once

#include "utils.h"
#include "camera.h"

class Player {
    
    vec3 position;
    vec3 direction;
    vec3 up;

public:
    Player() = default;
    Player(vec3 pos);
    void Update();
    Camera GetCamera();
};