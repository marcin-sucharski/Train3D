#version 130

in  vec3 in_pos;
in  vec3 in_color;

out vec3 ex_color;

void main(void) {
    gl_Position = vec4(in_pos, 1.0);
    ex_color = in_color;
}