#include "mesh-generator.h"
#include <vector>
#include <cassert>
#include "vertex-format.h"

namespace train {
    namespace res {
        MeshData<SimpleVertex> MeshGenerator::SampleTriangle() {
            std::vector<SimpleVertex> vertices = {
                {{0.0f,  0.5f,  0.0f}, {1.0f, 0.0f, 0.0f}},
                {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{0.5f,  -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            };

            return MeshData<SimpleVertex>(std::move(vertices));
        }

        MeshData<SimpleVertex> MeshGenerator::ColoredCube() {
            std::vector<SimpleVertex> pos = {
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

            return MeshData<SimpleVertex>(std::move(vertices));
        }

        MeshData<TexturedVertex> MeshGenerator::TexturedCube() {
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

        MeshData<GridVertex> MeshGenerator::Grid(int width, int height, float cellSize) {
            assert(width > 0 && "Width has to be greater than 0");
            assert(height > 0 && "Height has to be greater than 0");

            const int TriangleCount = 2 * width * height;
            const int VertexCount = 3 * TriangleCount;

            std::vector<GridVertex> vertices;
            vertices.reserve(VertexCount);

            const float StartX = -width * cellSize * 0.5f;
            const float StartZ = -height * cellSize * 0.5f;

            for (float z = StartZ; z < -StartZ; z += cellSize) {
                for (float x = StartX; x < -StartX; x += cellSize) {
                    GridVertex quad[] = {
                        {{x + cellSize, z}},
                        {{x,            z}},
                        {{x,            z + cellSize}},

                        {{x + cellSize, z}},
                        {{x,            z + cellSize}},
                        {{x + cellSize, z + cellSize}},
                    };

                    vertices.insert(
                        vertices.end(),
                        quad,
                        quad + sizeof(quad) / sizeof(quad[0])
                    );
                }
            }

            return MeshData<GridVertex>(std::move(vertices));
        }
    }
}