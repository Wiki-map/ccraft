#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "engine/window.h"

void UseCamera(Shader shader,Camera camera) {
    glm::vec3 glm_position = Vec3ToGLM(camera.position);
    glm::vec3 glm_target = Vec3ToGLM(camera.target);

    glm::mat4 view = glm::lookAt(glm_position,glm_target,glm::vec3(0,1,0));
    shader.SetMatrix4Uniform("u_view",view);

    glm::mat4 proj = glm::perspective(glm::radians(camera.fov),GetWindowAspect(),camera.near,camera.far);
    shader.SetMatrix4Uniform("u_projection",proj);
}