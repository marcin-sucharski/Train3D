#ifndef TRAIN_RES_MESH_DATA_H
#define TRAIN_RES_MESH_DATA_H

#include <utility>
#include "vertex-definition.h"

namespace train {
    namespace res {
        template<typename T>
        struct MeshData {
            MeshData() {}

            explicit MeshData(std::vector<T> &&vertices) : vertices(vertices) {}

            MeshData(const MeshData &) = delete;

            MeshData(MeshData &&other) {
                *this = std::move(other);
            }

            MeshData &operator=(const MeshData &) = delete;

            MeshData &operator=(MeshData &&other) {
                if (this != &other) {
                    vertices = std::move(other.vertices);
                }
                return *this;
            }

            static const VertexDefinition &vertexDefinition;
            std::vector<T> vertices;
        };

        template<typename T>
        const VertexDefinition &MeshData<T>::vertexDefinition = T::Definition;
    }
}

#endif
