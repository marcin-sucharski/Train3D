#version 130
#name "standard.frag"

in  vec3 ex_normal;
in  vec3 ex_pos;
in  vec2 ex_texc;

uniform sampler2D diffuse;

void main(void) {
    vec3 lightDir = normalize(vec3(1, 1.3, 1));
    float ambient = 0.1;

    vec4 color = texture(diffuse, ex_texc);

    float diffLight = max(dot(normalize(ex_normal), lightDir), 0);
    gl_FragColor = color * (ambient + diffLight);
}