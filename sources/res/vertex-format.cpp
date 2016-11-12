#include "vertex-format.h"

namespace train {
    namespace res {
        VertexDefinition SimpleVertex::Definition = {
            {"in_pos",   0, 3, GL_FLOAT},
            {"in_color", 1, 3, GL_FLOAT},
        };

        VertexDefinition GridVertex::Definition = {
            {"in_pos", 0, 2, GL_FLOAT},
        };

        VertexDefinition TexturedVertex::Definition = {
            {"in_pos",  0, 3, GL_FLOAT},
            {"in_texc", 1, 2, GL_FLOAT},
        };
    }
}