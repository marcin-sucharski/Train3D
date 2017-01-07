#version 130
#name "terrain.vert"

in  vec3 in_pos;
in  vec3 in_normal;
in  vec2 in_texc;

out vec3 ex_pos;
out vec3 ex_normal;
out vec2 ex_texc;

uniform mat4x4 projectionView;

void main(void) {
    ex_pos = in_pos;
    ex_normal = in_normal;
    ex_texc = in_texc;
    gl_Position = projectionView * vec4(in_pos, 1.0);
}
