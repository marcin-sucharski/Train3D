#ifndef TRAIN_RES_IMAGE_HEIGHT_PROVIDER_HPP
#define TRAIN_RES_IMAGE_HEIGHT_PROVIDER_HPP

#include "../util/height-provider.h"
#include "image-data.h"

namespace train {
    namespace res {
        class ImageHeightProvider : public util::HeightProvider {
            ImageData &imageData;
            float heightScale;

        public:
            ImageHeightProvider(ImageData &imageData, float heightScale);

            float getHeight(glm::vec2 pos) override;

        protected:
            glm::vec2 getElementSize() override;
        };
    }
}

#endif // TRAIN_RES_IMAGE_HEIGHT_PROVIDER_HPP
