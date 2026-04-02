#version 460
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in int a_texindex;
layout (location = 3) in vec3 a_norm;
layout (location = 4) in float a_tint;

out vec2 v_uv;
out int v_texindex;
out vec3 v_norm;
out float v_tint;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

void main() {
    gl_Position = vec4(a_pos,1.0) * u_view * u_projection;
    v_uv = a_uv;
    v_texindex = a_texindex;
    v_norm = a_norm;
    v_tint = a_tint;
}
