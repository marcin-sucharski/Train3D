#ifndef TRAIN_RES_MESH_DATA_H
#define TRAIN_RES_MESH_DATA_H

#include <utility>
#include <cassert>
#include "vertex-definition.h"

namespace train {
    namespace res {
        template<typename T>
        struct MeshData {
            MeshData() {}

            explicit MeshData(std::vector<T> vertices) : vertices(std::move(vertices)) {}

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

            MeshData merge(const MeshData &other) const {
                assert(vertexDefinition == other.vertexDefinition);

                std::vector<T> resultVertices;
                resultVertices.reserve(vertices.size() + other.vertices.size());
                resultVertices.insert(resultVertices.end(), vertices.begin(), vertices.end());
                resultVertices.insert(resultVertices.end(), other.vertices.begin(), other.vertices.end());
                return MeshData(std::move(resultVertices));
            }
        };

        template<typename T>
        const VertexDefinition &MeshData<T>::vertexDefinition = T::Definition;
    }
}

#endif
