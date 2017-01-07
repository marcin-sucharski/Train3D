#ifndef TRAIN_OBJ_TERRAIN_H
#define TRAIN_OBJ_TERRAIN_H

#include "object.h"
#include "../res/image-height-provider.h"
#include "../env/file.h"
#include "../gfx/shader-manager.h"
#include "../gfx/mesh.h"
#include "../gfx/texture.h"
#include <string>
#include <glm/glm.hpp>

namespace train {
    namespace obj {
        class Terrain : public Object {
            static const glm::vec3 scale;
            static const std::string heightmapPath;

            env::File textureFile;
            res::ImageData textureData;
            res::ImageHeightProvider heightProvider;

            gfx::Mesh mesh;
            gfx::ShaderProgram& shader;

            gfx::Texture grassTexture;
            gfx::Texture rockTexture;

        public:
            explicit Terrain(gfx::ShaderManager &shaderManager);

            util::HeightProvider& getHeightProvider();
            const glm::vec3& getScale() const;

            void update(double dt) override;
            void draw(const model::Camera &camera) override;

        private:
            static gfx::Mesh buildMesh(util::HeightProvider& heightProvider);
            static gfx::ShaderProgram& buildShaderProgram(gfx::ShaderManager &shaderManager);
        };
    }
}

#endif // TRAIN_OBJ_TERRAIN_H
