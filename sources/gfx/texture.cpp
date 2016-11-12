#include "texture.h"
#include "../env/file.h"
#include "../res/image-loader.h"
#include <cassert>

namespace train {
    namespace gfx {
        Texture::Texture() : handle(GL_FALSE) {
        }

        Texture::Texture(const void *data, int width, int height, GLenum format, const TextureParams &userParams) {
            TextureParams texParams = {
                {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
                {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
            };
            texParams.insert(texParams.end(), userParams.begin(), userParams.end());
            createTexture(data, width, height, format, texParams);
        }

        Texture::Texture(Texture &&texture) {
            *this = std::move(texture);
        }

        Texture::~Texture() {
            dispose();
        }

        Texture &Texture::operator=(Texture &&texture) {
            if (this != &texture) {
                handle = texture.getHandle();
            }

            return *this;
        }

        Texture Texture::fromFile(const char *filename) {
            env::File file(filename);
            if (!file.isValid()) {
                throw std::exception();
            }

            res::ImageData imgData = res::ImageLoader::load(file);
            return Texture(&imgData.data[0], imgData.width, imgData.height, imgData.format);
        }

        void Texture::createTexture(
            const void *data,
            int width,
            int height,
            GLenum format,
            const TextureParams &params
        ) {
            assert(data && "Data cannot be nullptr");
            assert(width > 0 && "Texture width cannot be less than 1");
            assert(height > 0 && "Texture height cannot be less than 1");

            glGenTextures(1, &handle);
            glBindTexture(GL_TEXTURE_2D, handle);

            for (const auto &p : params)
                glTexParameteri(GL_TEXTURE_2D, p.first, p.second);

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture::dispose() {
            if (handle) {
                glDeleteTextures(1, &handle);
                handle = 0;
            }
        }
    }
}