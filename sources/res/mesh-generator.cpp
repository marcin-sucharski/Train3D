#include "mesh-generator.h"
#include <vector>
#include <cassert>
#include <iostream>
#include "vertex-format.h"

using namespace std;

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

        MeshData<SimpleVertex> MeshGenerator::coloredCube(glm::vec3 position, float scale) {
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

        MeshData<TexturedVertex> MeshGenerator::texturedCube() {
            std::vector<TexturedVertex> pos = {
                {{-1.0f, 1.0f,  -1.0f}, {0.0f, 0.0f}},
                {{-1.0f, 1.0f,  1.0f},  {1.0f, 0.0f}},
                {{1.0f,  1.0f,  -1.0f}, {1.0f, 1.0f}},
                {{1.0f,  1.0f,  1.0f},  {0.0f, 1.0f}},

                {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f}},
                {{-1.0f, -1.0f, 1.0f},  {1.0f, 1.0f}},
                {{1.0f,  -1.0f, -1.0f}, {0.0f, 1.0f}},
                {{1.0f,  -1.0f, 1.0f},  {0.0f, 0.0f}},
            };

            std::vector<TexturedVertex> vertices{
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

            return MeshData<TexturedVertex>(std::move(vertices));
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
                        auto pos = glm::vec2(x[x_idx] / fw, z[z_idx] / fh);
                        return {
                            {x[x_idx], heightProvider.getHeight(pos) * heightScale, z[z_idx]},
                            heightProvider.getNormal(pos),
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

        MeshData<SimpleVertex> MeshGenerator::rails(util::CurveProvider &curveProvider) {
            const float tieDistance = 0.5f;
            const float width = 2.0f;
            const float baseStep = 0.005f;
            const float limit = 0.001f;

            float currentDistance = 0.0f;
            util::CurvePoint currentPoint = curveProvider.getPoint(currentDistance);
            auto getNextPoint = [&]() {
                float step = baseStep;
                float position = currentDistance, prevPosition;

                util::CurvePoint nextPoint;
                bool result;
                do {
                    prevPosition = position;
                    position += step;
                    position = glm::clamp(position, 0.0f, 1.0f);

                    nextPoint = curveProvider.getPoint(position);
                    const float distance = glm::distance(currentPoint.position, nextPoint.position);
                    result = glm::abs(distance - tieDistance) >= limit;

                    if (distance > tieDistance) {
                        position = prevPosition;
                        step *= 0.5f;
                    }

                    if (distance < tieDistance && position == 1.0f) {
                        break;
                    }
                } while (result);

                currentDistance = position;
                return nextPoint;
            };

            MeshData<SimpleVertex> result;
            while (glm::abs(currentDistance - 1.0f) > limit) {
                result.mergeInplace(coloredCube(currentPoint.position, 0.2f));
                currentPoint = getNextPoint();
            }

            return move(result);
        }
    }
}