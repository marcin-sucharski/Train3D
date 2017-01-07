#include "image-height-provider.h"

namespace train {
    namespace res {
        ImageHeightProvider::ImageHeightProvider(ImageData &imageData, float heightScale)
            : imageData(imageData), heightScale(heightScale) {
        }

        float ImageHeightProvider::getHeight(glm::vec2 pos) {
            const float exactPosX = pos.x * static_cast<float>(imageData.width + 1);
            const float exactPosY = pos.y * static_cast<float>(imageData.height + 1);

            const std::size_t x_lower = static_cast<std::size_t>(glm::floor(exactPosX));
            const std::size_t y_lower = static_cast<std::size_t>(glm::floor(exactPosY));

            const std::size_t x_upper = static_cast<std::size_t>(glm::ceil(exactPosX));
            const std::size_t y_upper = static_cast<std::size_t>(glm::ceil(exactPosY));

            auto getPixelHeight = [&](std::size_t x, std::size_t y) {
                const auto idx = y * imageData.width + x;
                const std::size_t bytesPerPixel = 3;
                const std::uint8_t rawValue = imageData.data[idx * bytesPerPixel];
                return static_cast<float>(rawValue) / std::numeric_limits<std::uint8_t>::max() * heightScale;
            };

            const float bottom[2] = {getPixelHeight(x_lower, y_lower), getPixelHeight(x_upper, y_lower)};
            const float top[2] = {getPixelHeight(x_lower, y_upper), getPixelHeight(x_upper, y_upper)};

            const float xInterp = exactPosX - glm::floor(exactPosX);
            const float yInterp = exactPosY - glm::floor(exactPosY);

            const float bottomValue = bottom[0] * (1.0f - xInterp) + bottom[1] * xInterp;
            const float topValue = top[0] * (1.0f - xInterp) + top[1] * xInterp;

            return bottomValue * (1.0f - yInterp) + topValue * yInterp;
        }

        glm::vec2 ImageHeightProvider::getElementSize() {
            return glm::vec2(1.0f / static_cast<float>(imageData.width), 1.0f / static_cast<float>(imageData.height));
        }
    }
}