#version 130

in  vec2 in_pos;

out vec3 ex_color;

uniform mat4x4 projectionView;

uniform vec3 terrainSize;
uniform sampler2D terrainTexture;

void main(void) {
    vec2 halfSize = terrainSize.xy / 2.0;
    vec2 coord = (in_pos + halfSize) * vec2(1.0 / terrainSize.x, 1.0 / terrainSize.y);
    float h = texture2DLod(terrainTexture, coord, 0.0).x;// * terrainSize.z;

    gl_Position = projectionView * vec4(in_pos.x, h * 256.0, in_pos.y, 1.0);
    //ex_color = texture(terrainTexture, coord).xyz;
    ex_color = vec3(h, h, h);
}