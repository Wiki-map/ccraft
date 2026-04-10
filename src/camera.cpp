#include "camera.h"
#include "engine/window.h"

void UseCamera(Shader shader,Camera camera) {

    mat4 view = lookat(camera.position,camera.target,vec3(0,1,0));
    shader.SetMatrix4Uniform("u_view",view);

    mat4 proj = perspective(radians(camera.fov),GetWindowAspect(),camera.near,camera.far);
    shader.SetMatrix4Uniform("u_projection",proj);
}