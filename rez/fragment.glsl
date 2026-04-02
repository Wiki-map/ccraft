#version 460

in vec2 v_uv;
flat in int v_texindex;
in vec3 v_norm;
in float v_tint;

out vec4 FragColor;

uniform sampler2D u_tex[8];

void main() {
    vec4 color = texture(u_tex[int(v_texindex)],v_uv);

    FragColor = color * vec4(v_tint);
}
