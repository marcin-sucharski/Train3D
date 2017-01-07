#version 130
#name "terrain.frag"

in  vec3 ex_normal;
in  vec3 ex_pos;
in  vec2 ex_texc;

uniform sampler2D textureGrass;
uniform sampler2D textureRock;

void main(void) {
    vec3 lightDir = normalize(vec3(1, 1.3, 1));
    float ambient = 0.1;

    vec4 grass = texture(textureGrass, ex_texc);
    vec4 rock = texture(textureRock, ex_texc);

    float rockPercent = pow(clamp(1.0 - dot(ex_normal, vec3(0.0, 1.0, 0.0)) + 0.7, 0.0, 1.0), 16);

    vec4 color = rock * rockPercent + grass * (1.0 - rockPercent);

    float diffLight = max(dot(normalize(ex_normal), lightDir), 0);
    gl_FragColor = color * (ambient + diffLight);
}