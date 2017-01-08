#ifndef TRAIN_RES_MESH_GENERATOR_H_H
#define TRAIN_RES_MESH_GENERATOR_H_H

#include "mesh-data.h"
#include "vertex-format.h"
#include "../util/curve-provider.h"
#include "../util/height-provider.h"

namespace train {
    namespace res {
        struct MeshGenerator {
            static MeshData<SimpleVertex> sampleTriangle();

            static MeshData<SimpleVertex> coloredCube(glm::vec3 position = glm::vec3(), float scale = 1.0f);

            static MeshData<StandardVertex> texturedCube(glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f));

            static MeshData<GridVertex> grid(int width, int height, float cellSize = 1.0);

            static MeshData<StandardVertex> terrain(
                int width,
                int height,
                util::HeightProvider &heightProvider,
                float heightScale,
                float cellSize = 1.0
            );

            static MeshData<StandardVertex> rails(util::CurveProvider &curveProvider);

            static MeshData<StandardVertex> cylinder(int elements, float radius, float height);
        };
    }
}

#endif
