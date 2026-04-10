#include "player.h"
#include "engine/window.h"
#include <cmath>

Player::Player(vec3 pos) {
    position = pos;
    up = {0,1,0};
    direction = {0,0,-1};

    pitch = 0;
    yaw = 0;

    last_mouse = {300,300};
    has_enterd = false;
}

void Player::Update(float dt) {

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

    direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));

    direction2.x = cos(radians(yaw));
    direction2.z = sin(radians(yaw));
    direction2.y = 0;

    float speed = 10;
    if (IsKeyDown(KeyboardKey::LEFT_CONTROL)) speed += 5;
    if (IsKeyDown(KeyboardKey::RIGHT_SHIFT)) speed -= 5;
    speed *= dt;

    if (IsKeyDown(KeyboardKey::W)) {
        position += direction2 * speed * 1.3;
    }
    if (IsKeyDown(KeyboardKey::S)) {
        position -= direction2 * speed * 1.3;
    }

    if (IsKeyDown(KeyboardKey::SPACE)) {
        position += up * speed;
    }
    if (IsKeyDown(KeyboardKey::LEFT_SHIFT)) {
        position -= up * speed;
    }

    if (IsKeyDown(KeyboardKey::A)) {
        vec3 right = normalize(cross(direction,up));
        position -= right * speed;
    }
    if (IsKeyDown(KeyboardKey::D)) {
        vec3 right = normalize(cross(direction,up));
        position += right * speed;
    }

    //std::cout<<position.x<<" "<<position.y<<" "<<position.z<<"\n";
}

Camera Player::GetCamera() {
    return {position,position + direction,60,0.1,10000};
}

vec3 Player::GetPosition() {
    return position;
}
vec3 Player::GetDirection() {
    return direction;
}
vec3 Player::GetRight() {
    return normalize(cross(direction,up));
}

vec3 Player::GetDirection2() {
    return direction2;
}