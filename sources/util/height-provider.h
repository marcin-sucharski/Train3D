#ifndef TRAIN_UTIL_HEIGHT_PROVIDER_HPP
#define TRAIN_UTIL_HEIGHT_PROVIDER_HPP

#include <glm/glm.hpp>

namespace train {
    namespace util {
        struct HeightProvider {
            virtual ~HeightProvider() {}

            /*
             * Provides height value for normalized coordinates (both in (0.0, 1.0) ).
             */
            virtual float getHeight(glm::vec2 pos) = 0;

            glm::vec3 getNormal(glm::vec2 pos);

        protected:
            virtual glm::vec2 getElementSize() = 0;
        };
    }
}

#endif // TRAIN_UTIL_HEIGHT_PROVIDER_HPP
