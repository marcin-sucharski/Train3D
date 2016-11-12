#include "mesh.h"
#include "../res/vertex-definition.h"
#include <utility>

namespace train {
    namespace gfx {
        Mesh::Mesh(GLuint vao, GLuint vbo, std::size_t vertexCount)
            : vao(vao), vbo(vbo), vertexCount(vertexCount) {
        }

        Mesh::Mesh(Mesh &&mesh) {
            *this = std::move(mesh);
        }

        Mesh::~Mesh() {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
        }

        Mesh &Mesh::operator=(Mesh &&mesh) {
            if (this != &mesh) {
                vao = mesh.vao;
                vbo = mesh.vbo;
            }

            return *this;
        }

        void Mesh::draw() {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
        }

        Mesh Mesh::create(
            const res::VertexDefinition &vertexDefinition,
            const void *vertices,
            std::size_t vertexCount
        ) {
            GLuint vao, vbo;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER,
                vertexCount * vertexDefinition.getSize(),
                vertices,
                GL_STATIC_DRAW);

            for (const auto &e : vertexDefinition.getElements()) {
                auto offset = (const GLvoid *) vertexDefinition.getOffset(e.index);
                glVertexAttribPointer(
                    e.index,
                    e.componentCount,
                    e.type,
                    GL_FALSE,
                    (GLsizei) vertexDefinition.getSize(),
                    offset);
                glEnableVertexAttribArray(e.index);
            }

            return Mesh(vao, vbo, vertexCount);
        }
    }
}