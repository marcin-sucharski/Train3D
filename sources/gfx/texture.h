#ifndef TRAIN_GFX_TEXTURE_H
#define TRAIN_GFX_TEXTURE_H

#include "../ogl.h"
#include <vector>

namespace train {
    namespace gfx {
        class Texture {
        public:
            /// Parameters passed to glTexParameteri function.
            typedef std::vector<std::pair<GLenum, GLint>> TextureParams;

        private:
            GLuint handle;

        public:
            Texture();
            Texture(const void *data, int width, int height, GLenum format, const TextureParams &userParams = {});
            Texture(const Texture &) = delete;
            Texture(Texture &&texture);
            ~Texture();

            Texture &operator=(const Texture &texture) = delete;
            Texture &operator=(Texture &&texture);

            inline bool isValid() const {
                return handle != 0;
            }

            inline GLuint getHandle() const {
                return handle;
            }

            static Texture fromFile(const char *filename, const TextureParams &userParams = {});

        private:
            void createTexture(const void *data, int width, int height, GLenum format, const TextureParams &params);
            void dispose();
        };
    }
}

#endif
