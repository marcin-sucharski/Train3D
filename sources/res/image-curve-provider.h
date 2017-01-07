#ifndef TRAIN_RES_IMAGE_CURVE_PROVIDER_H
#define TRAIN_RES_IMAGE_CURVE_PROVIDER_H

#include <glm/glm.hpp>
#include "../util/curve-provider.h"
#include "../util/height-provider.h"
#include "image-data.h"

namespace train {
    namespace res {
        class ImageCurveProvider : public util::CurveProvider {
            const glm::vec3 terrainScale;
            const std::vector<glm::vec2> shapePoints;
            util::HeightProvider &heightProvider;

        public:
            ImageCurveProvider(
                util::HeightProvider& heightProvider,
                glm::vec3 terrainScale,
                std::vector<glm::vec2> shapePoints
            );

        protected:
            util::CurvePoint getPointImpl(float t) override;

        private:
            glm::vec2 getImagePos(float t) const;
        };
    }
}

#endif // TRAIN_RES_IMAGE_CURVE_PROVIDER_H
