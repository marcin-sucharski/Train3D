#ifndef TRAIN_UTIL_CURVE_PROVIDER_HPP
#define TRAIN_UTIL_CURVE_PROVIDER_HPP

#include <glm/glm.hpp>

namespace train {
    namespace util {
        struct CurvePoint {
            glm::vec3 position;
            glm::vec3 up;
            glm::vec3 forward;
        };

        struct CurveProvider {
            virtual ~CurveProvider() {}

            /**
             * @param t Distance from curve beginning. From 0.0 to 1.0.
             * @return Point data.
             */
            CurvePoint getPoint(float t) {
                assert(t >= 0.0f && t <= 1.0f);
                return getPointImpl(t);
            }

        protected:
            virtual CurvePoint getPointImpl(float t) = 0;
        };
    }
}

#endif // TRAIN_UTIL_CURVE_PROVIDER_HPP
