#pragma once

#include "utils.h"
#include "camera.h"

class Player {
    
    vec3 position;
    vec3 direction;
    vec3 up;

    float pitch,yaw;

    vec2 last_mouse;
    bool has_enterd;

public:
    Player() = default;
    Player(vec3 pos);
    void Update(float dt);
    Camera GetCamera();

    vec3 GetPosition();
    vec3 GetDirection();
    vec3 GetRight();
};
