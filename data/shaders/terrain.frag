#version 130

precision highp float;

in  vec3 ex_color;
smooth in  vec3 ex_normal;

void main(void) {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float ambient = 0.1;

    gl_FragColor = vec4(ex_color, 1.0) * max(ambient, dot(normalize(ex_normal), lightDir));
    //gl_FragColor = vec4(ex_normal, 1.0);
}