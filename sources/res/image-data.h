#ifndef TRAIN_RES_IMAGE_DATA_H
#define TRAIN_RES_IMAGE_DATA_H

#include <vector>
#include <cstdint>
#include <utility>
#include "../ogl.h"

namespace train {
    namespace res {
        struct ImageData {
            ImageData() {}
            ImageData(const ImageData &) = delete;
            ImageData(ImageData &&imgData) {
                *this = std::move(imgData);
            }
            ImageData &operator=(const ImageData &) = delete;
            ImageData &operator=(ImageData &&imgData) {
                if (this != &imgData) {
                    data = std::move(imgData.data);
                    width = imgData.width;
                    height = imgData.height;
                    format = imgData.format;
                }

                return *this;
            }

            std::vector<std::uint8_t> data;
            std::int32_t width;
            std::int32_t height;
            GLenum format;
        };
    }
}

#endif
