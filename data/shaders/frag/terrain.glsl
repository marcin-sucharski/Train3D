#version 130
#name "terrain.frag"

precision highp float;

in  vec3 ex_color;
in  vec3 ex_normal;
in  vec3 ex_pos;

uniform vec3 lightPos;

void main(void) {
    vec3 lightDir = normalize(ex_pos - lightPos);
    float ambient = 0.1;

    gl_FragColor = vec4(ex_color, 1.0) * max(ambient, dot(normalize(ex_normal), lightDir));
    //gl_FragColor = vec4(ex_normal, 1.0);
}