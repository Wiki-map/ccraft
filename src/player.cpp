#include "player.h"
#include "engine/window.h"

Player::Player(vec3 pos) {
    position = pos;
    up = {0,1,0};
    direction = {0,0,-1};
}

void Player::Update() {

    float speed = 0.05;

    if (IsKeyDown(KeyboardKey::KEY_W)) {
        position.x += direction.x * speed;
        position.y += direction.y * speed;
        position.z += direction.z * speed;
    }
    if (IsKeyDown(KeyboardKey::KEY_S)) {
        position.x -= direction.x * speed;
        position.y -= direction.y * speed;
        position.z -= direction.z * speed;
    }

    if (IsKeyDown(KeyboardKey::KEY_SPACE)) {
        position.x += up.x * speed;
        position.y += up.y * speed;
        position.z += up.z * speed;
    }
    if (IsKeyDown(KeyboardKey::KEY_LEFT_SHIFT)) {
        position.x -= up.x * speed;
        position.y -= up.y * speed;
        position.z -= up.z * speed;
    }

    if (IsKeyDown(KeyboardKey::KEY_A)) {
        vec3 right = vec3Normalize(vec3Cross(direction,up));
        position.x -= right.x * speed;
        position.y -= right.y * speed;
        position.z -= right.z * speed;
    }
    if (IsKeyDown(KeyboardKey::KEY_D)) {
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