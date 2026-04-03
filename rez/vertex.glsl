#version 460
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec3 a_norm;
layout (location = 3) in float a_tint;

out vec4 v_color;
out vec3 v_norm;
out float v_tint;
out vec3 v_frag_pos;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

void main() {
    gl_Position = vec4(a_pos,1.0) * u_view * u_projection;
    v_color = a_color;
    v_norm = a_norm;
    v_tint = a_tint;
    v_frag_pos = a_pos;
}
