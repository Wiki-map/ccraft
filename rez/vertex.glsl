#version 410
layout (location = 0) in vec3 a_pos;
layout (location = 1) in uint a_color;
layout (location = 2) in uint a_norm;

out vec4 v_color;
out vec3 v_norm;
out vec3 v_frag_pos;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

vec3 normals[6] = vec3[](
    vec3(0,1,0),
    vec3(0,-1,0),
    vec3(-1,0,0),
    vec3(1,0,0),
    vec3(0,0,1),
    vec3(0,0,-1)
);

vec4 getcolor(uint id) {
    vec4 rez = vec4((a_color & uint(0x000000FF)) >> 0, (a_color & uint(0x0000FF00)) >> 8, (a_color & uint(0x00FF0000)) >> 16, (a_color & uint(0xFF000000))>> 24);
    rez.x /= 255.0f;
    rez.y /= 255.0f;
    rez.z /= 255.0f;
    rez.w /= 255.0f;
    return rez;
}

void main() {

    gl_Position = vec4(a_pos,1.0) * u_view * u_projection;
    v_color = getcolor(a_color);
    v_norm = normals[a_norm];
    v_frag_pos = a_pos;
}
