#include "circle-curve-provider.h"
#include <glm/gtc/constants.hpp>

namespace train {
    namespace util {
        CircleCurveProvider::CircleCurveProvider(glm::vec3 center, float radius)
            : center(center), radius(radius) {
        }

        CurvePoint CircleCurveProvider::getPointImpl(float t) {
            const float diff = 0.01;
            auto current = getCirclePoint(t);
            auto currentPlusDiff = getCirclePoint(t + diff);

            CurvePoint result;
            result.position = current;
            result.up = glm::vec3(0.0f, 1.0f, 0.0f);
            result.forward = glm::normalize(currentPlusDiff - current);

            return result;
        }

        glm::vec3 CircleCurveProvider::getCirclePoint(float t) const {
            float angle = t * 2.0f * glm::pi<float>();
            float x = glm::sin(angle) * radius;
            float y = glm::cos(angle) * radius;
            return center + glm::vec3(x, 0.0f, y);
        }
    }
}