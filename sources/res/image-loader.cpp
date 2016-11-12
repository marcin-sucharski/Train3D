#include "image-loader.h"
#include "../ogl.h"
#include <algorithm>

namespace train {
    namespace res {
        ImageData ImageLoader::load(env::File &source) {
            ImageData result;
            std::int32_t channels;

            const auto data = source.readBinaryToEnd();
            unsigned char *imgData = SOIL_load_image_from_memory(
                reinterpret_cast<const unsigned char*>(&data[0]),
                static_cast<int>(data.size()),
                &result.width,
                &result.height,
                &channels,
                SOIL_LOAD_RGB
            );
            result.format = GL_RGB;

            if (!imgData) {
                throw std::exception();
            }

            const std::size_t bpp = 3;
            const std::size_t imageSize = result.width * result.height * bpp;
            result.data.resize(imageSize);
            std::copy(imgData, imgData + imageSize, result.data.begin());
            SOIL_free_image_data(imgData);

            return std::move(result);
        }
    }
}