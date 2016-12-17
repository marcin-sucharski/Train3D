#version 130

in  vec2 in_pos;

out vec3 ex_color;
out vec3 ex_normal;
out vec3 ex_pos;

uniform mat4x4 projectionView;

uniform vec3 terrainSize;
uniform sampler2D terrainTexture;

uniform float normalCoefficient;

vec2 toHeightMapSpace(vec2 pos) {
    vec2 halfSize = terrainSize.xy / 2.0;
    return (pos + halfSize) * vec2(1.0 / terrainSize.x, 1.0 / terrainSize.y);
}

float height(vec2 coord) {
    return texture2DLod(terrainTexture, coord, 0.0).x;
}

void main(void) {
    vec2 coord = toHeightMapSpace(in_pos);
    float h = height(coord);

    vec3 off = vec3(0.1, 0.1, 0.0);
    float hL = height(toHeightMapSpace(in_pos - off.xz));
    float hR = height(toHeightMapSpace(in_pos + off.xz));
    float hD = height(toHeightMapSpace(in_pos - off.zy));
    float hU = height(toHeightMapSpace(in_pos + off.zy));

    vec3 N;
    N.x = hL - hR;
    N.z = hD - hU;
    N.y = normalCoefficient; // 0.001;
    N = -normalize(N);

    float resultY = h * terrainSize.z;
    gl_Position = projectionView * vec4(in_pos.x, resultY, in_pos.y, 1.0);
    ex_color = vec3(0.1, 0.8, 0.2);
    ex_normal = N;
    ex_pos = vec3(in_pos.x, resultY, in_pos.y);
}