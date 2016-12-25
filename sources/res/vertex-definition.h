#ifndef TRAIN_RES_VERTEX_DEFINITION_H
#define TRAIN_RES_VERTEX_DEFINITION_H

#include "../ogl.h"
#include <vector>
#include <string>

namespace train {
    namespace res {
        struct VertexElement {
            std::string name;
            std::uint32_t index;

            std::uint32_t componentCount;
            GLenum type;
        };

        bool operator ==(const VertexElement &a, const VertexElement &b);
        bool operator !=(const VertexElement &a, const VertexElement &b);

        class VertexDefinition {
            std::vector<VertexElement> elements;

        public:
            VertexDefinition(std::initializer_list<VertexElement> elems);
            VertexDefinition(const VertexDefinition &vertexDef);
            VertexDefinition(VertexDefinition &&vertexDef);
            ~VertexDefinition();

            VertexDefinition &operator=(const VertexDefinition &vDef);
            VertexDefinition &operator=(VertexDefinition &&vDef);

            std::uint32_t getElementsCount() const;
            std::size_t getSize() const;
            std::size_t getOffset(std::uint32_t elementIndex) const;

            const std::vector<VertexElement> &getElements() const;
        };

        bool operator ==(const VertexDefinition &a, const VertexDefinition &b);
        bool operator !=(const VertexDefinition &a, const VertexDefinition &b);
    }
}

#endif
