#ifndef TRAIN_UTIL_CIRCLE_CURVE_PROVIDER_H
#define TRAIN_UTIL_CIRCLE_CURVE_PROVIDER_H

#include <glm/glm.hpp>
#include "curve-provider.h"

namespace train {
    namespace util {
        class CircleCurveProvider : public CurveProvider {
            glm::vec3 center;
            float radius;

        public:
            CircleCurveProvider(glm::vec3 center, float radius);

        protected:
            CurvePoint getPointImpl(float t) override;

        private:
            glm::vec3 getCirclePoint(float t) const;
        };
    }
}

#endif // TRAIN_UTIL_CIRCLE_CURVE_PROVIDER_H
