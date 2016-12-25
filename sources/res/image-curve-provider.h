#ifndef TRAIN_RES_IMAGE_CURVE_PROVIDER_H
#define TRAIN_RES_IMAGE_CURVE_PROVIDER_H

#include <glm/glm.hpp>
#include "../util/curve-provider.h"
#include "image-data.h"

namespace train {
    namespace res {
        class ImageCurveProvider : public util::CurveProvider {
            const ImageData& imageData;
            const std::vector<glm::vec2> shapePoints;

        public:
            ImageCurveProvider(const ImageData& imageData, std::vector<glm::vec2> shapePoints);

        protected:
            util::CurvePoint getPointImpl(float t) override;

        private:
            float getHeight(glm::vec2 pos) const;
        };
    }
}

#endif // TRAIN_RES_IMAGE_CURVE_PROVIDER_H
