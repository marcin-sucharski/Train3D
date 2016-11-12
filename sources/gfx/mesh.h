#ifndef TRAIN_GFX_MESH_H
#define TRAIN_GFX_MESH_H

#include "../ogl.h"
#include <cstdint>
#include "../res/mesh-data.h"

namespace train {
    namespace res {
        class VertexDefinition;
    }

    namespace gfx {
        class Mesh {
            GLuint vao;
            GLuint vbo;

            std::size_t vertexCount;

        public:
            Mesh(const Mesh &) = delete;
            Mesh(Mesh &&mesh);
            ~Mesh();

            Mesh &operator=(const Mesh &) = delete;
            Mesh &operator=(Mesh &&mesh);

            inline bool isValid() const {
                return vao != 0;
            }

            void draw();

            static Mesh create(
                const res::VertexDefinition &vertexDefinition,
                const void *vertices,
                std::size_t vertexCount
            );

            template <typename VertexFormat>
            static Mesh create(const res::MeshData<VertexFormat> &data) {
                return create(
                    data.vertexDefinition,
                    &data.vertices[0],
                    data.vertices.size()
                );
            }

        private:
            Mesh(GLuint vao, GLuint vbo, std::size_t vertexCount);
        };
    }
}

#endif
