#version 460

in vec4 v_color;
in vec3 v_norm;
in float v_tint;
in vec3 v_frag_pos;

out vec4 FragColor;

uniform sampler2D u_tex[8];
uniform vec3 u_light_pos;

void main() {
    vec4 color = v_color * vec4(v_tint);

    vec3 ambient = vec3(0.7);

    vec3 norm = normalize(v_norm);
    vec3 lightDir = normalize(u_light_pos - v_frag_pos);

    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = vec3(diff);

    vec4 result = vec4(diffuse,1.0f);

    if (length(result) < 1.5) result += vec4(ambient,1.0f);

    FragColor = color * result;
}
