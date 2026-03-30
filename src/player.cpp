#include "player.h"
#include "engine/window.h"
#include "iostream"

Player::Player(vec3 pos) {
    position = pos;
    up = {0,1,0};
    direction = {0,0,-1};

    pitch = 0;
    yaw = 0;

    last_mouse = {300,300};
    has_enterd = false;
}

static int sigen(float val) {
    return val / std::abs(val);
}

void Player::Update() {

    if (IsKeyDown(KeyboardKey::C)) {
        CenterMouse();
        last_mouse = {(float)GetWindowWidth()/2,(float)GetWindowHeight()/2};
        pitch = 0;
        yaw = 90;
    }

    vec2 mouse = GetMousePosition();

    int offx = mouse.x - last_mouse.x;
    int offy = last_mouse.y - mouse.y;

    last_mouse = mouse;

    float sens = 0.1f;

    pitch += offy * sens;
    yaw += offx * sens;

    if (pitch > 89) pitch = 89;
    if (pitch < -89) pitch = -89;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    float speed = 0.05;

    if (IsKeyDown(KeyboardKey::W)) {
        position.x += cos(glm::radians(yaw)) * speed * 1.3;
        position.y += 0 * speed;
        position.z += sin(glm::radians(yaw)) * speed * 1.3;
    }
    if (IsKeyDown(KeyboardKey::S)) {
        position.x -= cos(glm::radians(yaw)) * speed * 1.3;
        position.y -= 0 * speed;
        position.z -= sin(glm::radians(yaw)) * speed * 1.3;
    }

    if (IsKeyDown(KeyboardKey::SPACE)) {
        position.x += up.x * speed;
        position.y += up.y * speed;
        position.z += up.z * speed;
    }
    if (IsKeyDown(KeyboardKey::LEFT_SHIFT)) {
        position.x -= up.x * speed;
        position.y -= up.y * speed;
        position.z -= up.z * speed;
    }

    if (IsKeyDown(KeyboardKey::A)) {
        vec3 right = vec3Normalize(vec3Cross(direction,up));
        position.x -= right.x * speed;
        position.y -= right.y * speed;
        position.z -= right.z * speed;
    }
    if (IsKeyDown(KeyboardKey::D)) {
        vec3 right = vec3Normalize(vec3Cross(direction,up));
        position.x += right.x * speed;
        position.y += right.y * speed;
        position.z += right.z * speed;
    }
}

Camera Player::GetCamera() {
    vec3 target = position;
    target.x += direction.x;
    target.y += direction.y;
    target.z += direction.z;
    return {position,target,60,0.1,100};
}