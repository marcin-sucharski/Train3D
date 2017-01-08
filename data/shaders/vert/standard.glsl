#version 130
#name "standard.vert"

in  vec3 in_pos;
in  vec3 in_normal;
in  vec2 in_texc;

out vec3 ex_pos;
out vec3 ex_normal;
out vec2 ex_texc;

uniform mat4x4 projectionView;
uniform mat4x4 model;
uniform mat4x4 invModel;

void main(void) {
    ex_pos = in_pos;
    ex_normal = (invModel * vec4(in_normal, 1.0)).xyz;
    ex_texc = in_texc;
    gl_Position = projectionView * model * vec4(in_pos, 1.0);
}
