#include "mesh-generator.h"
#include <vector>
#include <cassert>
#include "vertex-format.h"
#include "../util/curve-iterator.h"

using namespace std;
using namespace glm;

namespace train {
    namespace res {
        MeshData<SimpleVertex> MeshGenerator::sampleTriangle() {
            std::vector<SimpleVertex> vertices = {
                {{0.0f,  0.5f,  0.0f}, {1.0f, 0.0f, 0.0f}},
                {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            };

            return MeshData<SimpleVertex>(std::move(vertices));
        }

        MeshData<SimpleVertex> MeshGenerator::coloredCube(vec3 position, float scale) {
            const std::vector<SimpleVertex> pos = {
                {{-1.0f, 1.0f,  -1.0f}, {1.0f, 0.0f, 0.0f}},
                {{-1.0f, 1.0f,  1.0f},  {0.0f, 1.0f, 0.0f}},
                {{1.0f,  1.0f,  -1.0f}, {0.0f, 0.0f, 1.0f}},
                {{1.0f,  1.0f,  1.0f},  {1.0f, 0.0f, 0.0f}},

                {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
                {{-1.0f, -1.0f, 1.0f},  {1.0f, 0.0f, 0.0f}},
                {{1.0f,  -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
                {{1.0f,  -1.0f, 1.0f},  {0.0f, 1.0f, 0.0f}},
            };

            std::vector<SimpleVertex> vertices{
                // top
                pos[1], pos[2], pos[0],
                pos[3], pos[2], pos[1],

                // bottom
                pos[4], pos[6], pos[5],
                pos[5], pos[6], pos[7],

                // front
                pos[0], pos[6], pos[4],
                pos[2], pos[6], pos[0],

                // back
                pos[5], pos[7], pos[1],
                pos[1], pos[7], pos[3],

                // left
                pos[5], pos[0], pos[4],
                pos[1], pos[0], pos[5],

                // right
                pos[2], pos[7], pos[6],
                pos[3], pos[7], pos[2],
            };

            for (auto &vertex : vertices)
                vertex.position = (vertex.position * scale + position);

            return MeshData<SimpleVertex>(std::move(vertices));
        }

        MeshData<StandardVertex> MeshGenerator::texturedCube(glm::vec3 scale) {
            std::vector<vec3> pos = {
                {-1.0f, 1.0f,  -1.0f},
                {-1.0f, 1.0f,  1.0f},
                {1.0f,  1.0f,  -1.0f},
                {1.0f,  1.0f,  1.0f},

                {-1.0f, -1.0f, -1.0f},
                {-1.0f, -1.0f, 1.0f},
                {1.0f,  -1.0f, -1.0f},
                {1.0f,  -1.0f, 1.0f},
            };

            for (auto &p : pos) {
                p.x *= scale.x;
                p.y *= scale.y;
                p.z *= scale.z;
            }

            const glm::vec2 texc[] = {
                {0.0f, 0.0f},
                {1.0f, 0.0f},
                {0.0f, 1.0f},
                {1.0f, 1.0f}
            };

            const glm::vec3 normal[] = {
                {0.f, 1.f, 0.f},
                {0.f, -1.f, 0.f},

                {0.f, 0.f, -1.f},
                {0.f, 0.f, 1.f},

                {-1.f, 0.f, 0.f},
                {1.f, 0.f, 0.f}
            };

            std::vector<StandardVertex> vertices{
                // top
                {pos[1], normal[0], texc[0]}, {pos[2], normal[0], texc[3]}, {pos[0], normal[0], texc[1]},
                {pos[3], normal[0], texc[2]}, {pos[2], normal[0], texc[3]}, {pos[1], normal[0], texc[0]},

                // bottom
                {pos[4], normal[1], texc[0]}, {pos[6], normal[1], texc[1]}, {pos[5], normal[1], texc[3]},
                {pos[5], normal[1], texc[3]}, {pos[6], normal[1], texc[1]}, {pos[7], normal[1], texc[2]},

                // front
                {pos[0], normal[2], texc[1]}, {pos[6], normal[2], texc[3]}, {pos[4], normal[2], texc[2]},
                {pos[2], normal[2], texc[1]}, {pos[6], normal[2], texc[2]}, {pos[0], normal[2], texc[0]},

                // back
                {pos[5], normal[3], texc[2]}, {pos[7], normal[3], texc[3]}, {pos[1], normal[3], texc[1]},
                {pos[1], normal[3], texc[1]}, {pos[7], normal[3], texc[3]}, {pos[3], normal[3], texc[0]},

                // left
                {pos[5], normal[4], texc[2]}, {pos[0], normal[4], texc[1]}, {pos[4], normal[4], texc[3]},
                {pos[1], normal[4], texc[0]}, {pos[0], normal[4], texc[1]}, {pos[5], normal[4], texc[2]},

                // right
                {pos[2], normal[5], texc[0]}, {pos[7], normal[5], texc[3]}, {pos[6], normal[5], texc[3]},
                {pos[3], normal[5], texc[1]}, {pos[7], normal[5], texc[3]}, {pos[2], normal[5], texc[0]},
            };

            return MeshData<StandardVertex>(move(vertices));
        }

        namespace {
            const std::size_t VertexCountInQuad = 6;

            template<typename VertexType, typename QuadGen>
            MeshData<VertexType> gridGen(int width, int height, float cellSize, QuadGen quadGen) {
                assert(width > 0 && "Width has to be greater than 0");
                assert(height > 0 && "Height has to be greater than 0");

                const std::size_t TriangleCount = static_cast<size_t>(2 * width * height);
                const std::size_t VertexCount = 3 * TriangleCount;

                std::vector<VertexType> vertices;
                vertices.reserve(VertexCount);

                const float SizeX = width * cellSize;
                const float SizeZ = height * cellSize;

                for (float z = 0.0f; z < SizeZ; z += cellSize) {
                    for (float x = 0.0f; x < SizeX; x += cellSize) {
                        VertexType quad[VertexCountInQuad];
                        quadGen(x, z, quad);

                        vertices.insert(
                            vertices.end(),
                            quad,
                            quad + sizeof(quad) / sizeof(quad[0])
                        );
                    }
                }

                return MeshData<VertexType>(std::move(vertices));
            }
        }

        MeshData<GridVertex> MeshGenerator::grid(int width, int height, float cellSize) {
            return gridGen<GridVertex>(width, height, cellSize,
                [cellSize](float x, float z, GridVertex result[VertexCountInQuad]) {
                    GridVertex temp[VertexCountInQuad] = {
                        {{x + cellSize, z}},
                        {{x,            z}},
                        {{x,            z + cellSize}},

                        {{x + cellSize, z}},
                        {{x,            z + cellSize}},
                        {{x + cellSize, z + cellSize}},
                    };
                    std::copy(temp, temp + VertexCountInQuad, result);
                });
        }

        MeshData<StandardVertex> MeshGenerator::terrain(
            int width,
            int height,
            util::HeightProvider &heightProvider,
            float heightScale,
            float cellSize
        ) {
            float fw = static_cast<float>(width + 1), fh = static_cast<float>(height + 1);
            return gridGen<StandardVertex>(width, height, cellSize,
                [&](float x1, float z1, StandardVertex result[VertexCountInQuad]) {
                    float x[2] = {x1, x1 + cellSize};
                    float z[2] = {z1, z1 + cellSize};
                    auto genVertex = [&](int x_idx, int z_idx) -> StandardVertex {
                        auto pos = vec2(x[x_idx] / fw, z[z_idx] / fh);
                        return {
                            {x[x_idx], heightProvider.getHeight(pos) * heightScale, z[z_idx]},
                            heightProvider.getNormal(pos, vec3(width, heightScale, height)),
                            {static_cast<float>(x_idx), static_cast<float>(z_idx)}
                        };
                    };
                    
                    float x2 = x1 + cellSize;
                    auto z2 = z1 + cellSize;
                    StandardVertex temp[VertexCountInQuad] = {
                        genVertex(1, 0),
                        genVertex(0, 0),
                        genVertex(0, 1),

                        genVertex(1, 0),
                        genVertex(0, 1),
                        genVertex(1, 1)
                    };
                    std::copy(temp, temp + VertexCountInQuad, result);
                });
        }

        MeshData<StandardVertex> MeshGenerator::rails(util::CurveProvider &curveProvider) {
            const float tieDistance = 0.5f;
            const float width = 2.0f;

            util::CurveIterator points(curveProvider);

            const float tieWidth = 1.0f, tieHeight = 0.2;
            auto buildTie = [&](const util::CurvePoint &p) {
                const auto dir = normalize(cross(p.forward, p.up));
                const auto halfUp = p.up * 0.5f * tieHeight;
                const auto halfForward = p.forward * 0.5f * tieHeight;
                const auto halfDir = dir * 0.5f * tieWidth;

                const glm::vec3 off[4] = {
                    -halfUp - halfForward,
                    +halfUp - halfForward,
                    +halfUp + halfForward,
                    -halfUp + halfForward
                };

                vec3 left[4], right[4];
                for (int i = 0; i < 4; ++i) {
                    left[i] = p.position - halfDir + off[i];
                    right[i] = p.position + halfDir + off[i];
                }

                vector<StandardVertex> vertices = {
                    {right[2], dir, vec2(0.4f, 0.4f)},
                    {right[1], dir, vec2(0.0f, 0.4f)},
                    {right[0], dir, vec2(0.0f, 1.0f)},

                    {right[3], dir, vec2(0.4f, 1.0f)},
                    {right[2], dir, vec2(0.4f, 0.4f)},
                    {right[0], dir, vec2(0.0f, 1.0f)},


                    {left[0], -dir, vec2(0.0f, 1.0f)},
                    {left[1], -dir, vec2(0.0f, 0.4f)},
                    {left[2], -dir, vec2(0.4f, 0.4f)},

                    {left[0], -dir, vec2(0.0f, 1.0f)},
                    {left[2], -dir, vec2(0.4f, 0.4f)},
                    {left[3], -dir, vec2(0.4f, 1.0f)},


                    {left[1], -p.forward, vec2(1.0f, 0.4f)},
                    {left[0], -p.forward, vec2(1.0f, 1.0f)},
                    {right[0], -p.forward, vec2(0.0f, 1.0f)},

                    {right[1], -p.forward, vec2(0.0f, 0.4f)},
                    {left[1], -p.forward, vec2(1.0f, 0.4f)},
                    {right[0], -p.forward, vec2(0.0f, 1.0f)},


                    {left[2], p.up, vec2(1.0f, 0.4f)},
                    {left[1], p.up, vec2(1.0f, 1.0f)},
                    {right[1], p.up, vec2(0.0f, 1.0f)},

                    {right[2], p.up, vec2(0.0f, 0.4f)},
                    {left[2], p.up, vec2(1.0f, 0.4f)},
                    {right[1], p.up, vec2(0.0f, 1.0f)},


                    {left[3], p.forward, vec2(1.0f, 0.4f)},
                    {left[2], p.forward, vec2(1.0f, 1.0f)},
                    {right[2], p.forward, vec2(0.0f, 1.0f)},

                    {right[3], p.forward, vec2(0.0f, 0.4f)},
                    {left[3], p.forward, vec2(1.0f, 0.4f)},
                    {right[2], p.forward, vec2(0.0f, 1.0f)},


                    {right[0], -p.up, vec2(1.0f, 0.4f)},
                    {left[3], -p.up, vec2(1.0f, 1.0f)},
                    {right[3], -p.up, vec2(0.0f, 1.0f)},

                    {right[0], -p.up, vec2(0.0f, 0.4f)},
                    {left[0], -p.up, vec2(1.0f, 0.4f)},
                    {left[3], -p.up, vec2(0.0f, 1.0f)}
                };

                return MeshData<StandardVertex>(move(vertices));
            };
            const float railHeight = 0.1f, railSpacing = 0.75f;
            auto buildRailElement = [&](const util::CurvePoint &curr, const util::CurvePoint &next) {
                auto getEndpointVertices = [&](
                    const util::CurvePoint &p,
                    vec3 outLeft[4],
                    vec3 outRight[4]
                ) {
                    const auto dir = normalize(cross(p.forward, p.up));
                    const auto halfUp = p.up * 0.5f * railHeight;
                    const auto halfSide = dir * 0.5f * railHeight;
                    const auto halfDir = dir * 0.5f * tieWidth * railSpacing;
                    const auto halfTieHeight = p.up * tieHeight * 0.5f;

                    const vec3 off[4] = {
                        -halfUp - halfSide,
                        +halfUp - halfSide,
                        +halfUp + halfSide,
                        -halfUp + halfSide
                    };
                    for (int i = 0; i < 4; ++i) {
                        outLeft[i] = p.position + halfTieHeight - halfDir + off[i];
                        outRight[i] = p.position + halfTieHeight + halfDir + off[i];
                    }
                };
                auto mergeEndpoints = [](const vec3 begin[4], const vec3 end[4]) {
                    vector<StandardVertex> vertices;
                    vertices.reserve(4 * 6);
                    for (int i = 0; i < 4; ++i) {
                        const auto j = (i + 1) % 4;
                        const vec3 normal = normalize(cross(end[j] - begin[i], end[i] - begin[i]));
                        StandardVertex quad[6] = {
                            {end[j], normal, vec2(1.0f, 0.4f)},
                            {end[i], normal, vec2(1.0f, 0.0f)},
                            {begin[i], normal, vec2(0.0f, 0.0f)},

                            {begin[j], normal, vec2(0.0f, 0.4f)},
                            {end[j], normal, vec2(1.0f, 0.4f)},
                            {begin[i], normal, vec2(0.0f, 0.0f)},
                        };
                        vertices.insert(vertices.end(), quad, quad + 6);
                    }

                    return MeshData<StandardVertex>(move(vertices));
                };

                vec3 first[2][4], second[2][4];
                getEndpointVertices(curr, first[0], first[1]);
                getEndpointVertices(next, second[0], second[1]);

                MeshData<StandardVertex> result;
                result.mergeInplace(mergeEndpoints(first[0], second[0]));
                result.mergeInplace(mergeEndpoints(first[1], second[1]));
                return move(result);
            };

            MeshData<StandardVertex> result;
            auto currentPoint = points.getCurrentPoint();
            do {
                auto nextPoint = points.getNext(tieDistance);
                result.mergeInplace(buildTie(currentPoint));
                result.mergeInplace(buildRailElement(currentPoint, nextPoint));
                currentPoint = nextPoint;
            } while (points.hasNext());
            result.mergeInplace(buildRailElement(currentPoint, curveProvider.getPoint(0.0f)));

            return move(result);
        }

        MeshData<StandardVertex> MeshGenerator::cylinder(int elements, float radius, float height) {
            vector<StandardVertex> vertices;

            const float angleChange = 2.f * pi<float>() / static_cast<float>(elements);
            for (int i = 0; i < elements; ++i) {
                const auto first = vec2(sin(i * angleChange), cos(i * angleChange));
                const auto second = vec2(sin((i + 1) * angleChange), cos((i + 1) * angleChange));

                const auto f = first * radius, s = second * radius;

                const auto fN = normalize(vec3(f.x, 0.f, f.y)), sN = normalize(vec3(s.x, 0.f, s.y));

                auto texcOff = vec2(0.5, 0.5);
                StandardVertex circleFace[] = {
                    {vec3(s.x, 0.f, s.y), vec3(0.f, -1.f, 0.f),   second + texcOff},
                    {vec3(f.x, 0.f, f.y), vec3(0.f, -1.f, 0.f),   first + texcOff},
                    {vec3(), vec3(0.f, -1.f, 0.f),                texcOff},

                    {vec3(0.f, height, 0.f), vec3(0.f, 1.f, 0.f), texcOff},
                    {vec3(f.x, height, f.y), vec3(0.f, 1.f, 0.f), first + texcOff},
                    {vec3(s.x, height, s.y), vec3(0.f, 1.f, 0.f), second + texcOff}
                };
                StandardVertex sideFace[] = {
                    {vec3(f.x, 0.f, f.y), fN, vec2(0.f, 0.f)},
                    {vec3(s.x, 0.f, s.y), sN, vec2(0.f, 0.1f)},
                    {vec3(f.x, height, f.y), fN, vec2(1.f, 0.f)},

                    {vec3(s.x, height, s.y), sN, vec2(1.f, 0.1f)},
                    {vec3(f.x, height, f.y), fN, vec2(1.f, 0.f)},
                    {vec3(s.x, 0.f, s.y), sN, vec2(0.f, 0.1f)}
                };

                vertices.insert(vertices.end(), circleFace, circleFace + sizeof(circleFace) / sizeof(circleFace[0]));
                vertices.insert(vertices.end(), sideFace, sideFace + sizeof(sideFace) / sizeof(sideFace[0]));
            }

            return MeshData<StandardVertex>(move(vertices));
        }
    }
}