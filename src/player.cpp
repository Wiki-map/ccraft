#include "player.h"
#include "engine/window.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

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

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    direction2.x = cos(glm::radians(yaw));
    direction2.z = sin(glm::radians(yaw));
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
        vec3 right = vec3Normalize(vec3Cross(direction,up));
        position -= right * speed;
    }
    if (IsKeyDown(KeyboardKey::D)) {
        vec3 right = vec3Normalize(vec3Cross(direction,up));
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
    return vec3Normalize(vec3Cross(direction,up));
}

vec3 Player::GetDirection2() {
    return direction2;
}

glm::mat4 Player::GetViewMatrix() {
    Camera camera = GetCamera();
    glm::vec3 glm_position = Vec3ToGLM(camera.position);
    glm::vec3 glm_target = Vec3ToGLM(camera.target);

    glm::mat4 view = glm::lookAt(glm_position,glm_target,glm::vec3(0,1,0));

    return view;
}

glm::mat4 Player::GetPerspectivMatrix() {
    Camera camera = GetCamera();
    glm::vec3 glm_position = Vec3ToGLM(camera.position);
    glm::vec3 glm_target = Vec3ToGLM(camera.target);

    glm::mat4 proj = glm::perspective(glm::radians(camera.fov),GetWindowAspect(),camera.near,camera.far);
    return proj;
}
