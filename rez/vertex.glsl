#version 460
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in float a_texindex;

out vec2 v_uv;
out float v_texindex;

//uniform mat4 u_view;
//uniform mat4 u_projection;
//uniform mat4 u_model;

void main() {
    gl_Position = vec4(a_pos,1.0);
    v_uv = a_uv;
    v_texindex = a_texindex;
}