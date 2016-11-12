#ifndef TRAIN_RES_VERTEX_FORMAT_H
#define TRAIN_RES_VERTEX_FORMAT_H

#include <glm/glm.hpp>
#include "vertex-definition.h"

namespace train {
    namespace res {
        struct SimpleVertex {
            glm::vec3 position;
            glm::vec3 color;

            static VertexDefinition Definition;
        };

        struct GridVertex {
            glm::vec2 position;

            static VertexDefinition Definition;
        };

        struct TexturedVertex {
            glm::vec3 position;
            glm::vec2 textureCoordinate;

            static VertexDefinition Definition;
        };
    }
}

#endif
