#ifndef TRAIN_RES_MESH_GENERATOR_H_H
#define TRAIN_RES_MESH_GENERATOR_H_H

#include "mesh-data.h"
#include "vertex-format.h"
#include "../util/curve-provider.h"

namespace train {
    namespace res {
        struct MeshGenerator {
            /// Generates testing triangle.
            static MeshData<SimpleVertex> SampleTriangle();

            /// Generates colored cube.
            static MeshData<SimpleVertex> ColoredCube(glm::vec3 position = glm::vec3(), float scale = 1.0f);

            /// Generates textured cube.
            static MeshData<TexturedVertex> TexturedCube();

            /// Generates grid.
            static MeshData<GridVertex> Grid(int width, int height, float cellSize = 1.0);

            static MeshData<SimpleVertex> rails(util::CurveProvider &curveProvider);
        };
    }
}

#endif
