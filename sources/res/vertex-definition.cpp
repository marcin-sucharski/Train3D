#include "vertex-definition.h"
#include <string>
#include <cstdint>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

namespace train {
    namespace res {
        std::map<GLenum, std::size_t> GLFormatToSize = {
            {GL_BYTE,           sizeof(char)},
            {GL_UNSIGNED_BYTE,  sizeof(unsigned char)},
            {GL_SHORT,          sizeof(short)},
            {GL_UNSIGNED_SHORT, sizeof(unsigned short)},
            {GL_INT,            sizeof(int)},
            {GL_UNSIGNED_INT,   sizeof(unsigned int)},
            {GL_FLOAT,          sizeof(float)},
            {GL_DOUBLE,         sizeof(double)},
        };

        VertexDefinition::VertexDefinition(std::initializer_list<VertexElement> elems)
            : elements(elems) {
            std::sort(elements.begin(), elements.end(), [](const VertexElement &e1, const VertexElement &e2) {
                return e1.index < e2.index;
            });
        }

        VertexDefinition::VertexDefinition(const VertexDefinition &vertexDef) :
            elements(vertexDef.getElements()) {
        }

        VertexDefinition::VertexDefinition(VertexDefinition &&vertexDef) {
            (*this) = std::move(vertexDef);
        }

        VertexDefinition::~VertexDefinition() {
        }

        VertexDefinition &VertexDefinition::operator=(const VertexDefinition &vDef) {
            if (this != &vDef) {
                elements = vDef.getElements();
            }

            return *this;
        }

        VertexDefinition &VertexDefinition::operator=(VertexDefinition &&vDef) {
            if (this != &vDef) {
                elements = std::move(vDef.elements);
            }

            return *this;
        }

        std::uint32_t VertexDefinition::getElementsCount() const {
            return static_cast<std::uint32_t>(elements.size());
        }

        std::size_t VertexDefinition::getSize() const {
            std::size_t size = 0;

            for (const auto &v : elements)
                size += GLFormatToSize[v.type] * v.componentCount;

            return size;
        }

        std::size_t VertexDefinition::getOffset(std::uint32_t elementIndex) const {
            std::size_t offset = 0;

            for (const auto &v : elements) {
                if (v.index == elementIndex)
                    return offset;

                offset += GLFormatToSize[v.type] * v.componentCount;
            }

            return std::numeric_limits<std::size_t>::max();
        }

        const std::vector<VertexElement> &VertexDefinition::getElements() const {
            return elements;
        }
    }
}