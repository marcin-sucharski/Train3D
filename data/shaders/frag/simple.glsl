#version 130

precision highp float;

in  vec3 ex_color;

void main(void) {
    gl_FragColor = vec4(ex_color, 1.0);
}