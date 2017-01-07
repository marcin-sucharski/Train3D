#name "terrain.glsl"

vec2 toHeightMapSpace(vec2 pos, vec3 terrainSize) {
    vec2 halfSize = terrainSize.xy / 2.0;
    return pos * vec2(1.0 / terrainSize.x, 1.0 / terrainSize.y);
}

float height(vec2 coord, sampler2D terrainTexture) {
    return texture2DLod(terrainTexture, coord, 0.0).x;
}
