#include "terrain.h"
#include "../env/file.h"
#include "../res/image-data.h"
#include "../res/image-loader.h"
#include "../res/image-height-provider.h"
#include "../res/mesh-generator.h"

namespace train {
    namespace obj {
        const glm::vec3 Terrain::scale(256.f, 48.f, 256.f);
        const std::string Terrain::heightmapPath = "data/textures/terrain.png";

        Terrain::Terrain(gfx::ShaderManager &shaderManager)
            : textureFile(heightmapPath),
              textureData(train::res::ImageLoader::load(textureFile)),
              heightProvider(textureData),
              mesh(buildMesh(heightProvider)),
              shader(buildShaderProgram(shaderManager)),
              grassTexture(gfx::Texture::fromFile("data/textures/grass_grass_0124_01_tiled_s.jpg")),
              rockTexture(gfx::Texture::fromFile("data/textures/ground_stone_ground_0002_02_tiled_s.jpg")) {
        }

        util::HeightProvider &Terrain::getHeightProvider() {
            return heightProvider;
        }

        const glm::vec3 &Terrain::getScale() const {
            return scale;
        }

        void Terrain::update(double dt) {
        }

        void Terrain::draw(const model::Camera &camera) {
            shader.bind();
            shader.setUniform("projectionView", camera.getProjectionView());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, grassTexture.getHandle());
            shader.setUniform("textureGrass", 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, rockTexture.getHandle());
            shader.setUniform("textureRock", 1);

            mesh.draw();
            shader.unbind();
        }

        gfx::Mesh Terrain::buildMesh(util::HeightProvider& heightProvider) {
            auto terrainMeshData = res::MeshGenerator::terrain(
                static_cast<int>(scale.x),
                static_cast<int>(scale.z),
                heightProvider,
                scale.y
            );
            return train::gfx::Mesh::create(terrainMeshData);
        }

        gfx::ShaderProgram& Terrain::buildShaderProgram(gfx::ShaderManager &shaderManager) {
            return shaderManager.get(
                "vert/standard.glsl",
                "frag/terrain.glsl",
                res::StandardVertex::Definition
            );
        }
    }
}