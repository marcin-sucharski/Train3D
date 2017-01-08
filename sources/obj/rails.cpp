#include "rails.h"
#include "../res/image-curve-provider.h"
#include "../res/mesh-generator.h"

namespace train {
    namespace obj {
        Rails::Rails(gfx::ShaderManager &shaderManager, Terrain &terrain)
            : curveProvider(
            new train::res::ImageCurveProvider(
                terrain.getHeightProvider(),
                terrain.getScale(),
                prepareShapePoints()
            )
        ),
              mesh(buildMesh(*curveProvider)),
              shader(buildShader(shaderManager)),
              texture(gfx::Texture::fromFile("data/textures/rails.png")) {
        }

        util::CurveProvider &Rails::getCurve() {
            return *curveProvider;
        }

        void Rails::update(double dt) {
        }

        void Rails::draw(const model::Camera &camera) {
            shader.bind();
            shader.setUniform("projectionView", camera.getProjectionView());
            shader.setUniform("model", glm::mat4x4());
            shader.setUniform("invModel", glm::mat4x4());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.getHandle());
            shader.setUniform("diffuse", 0);

            mesh.draw();
            shader.unbind();
        }

        gfx::Mesh Rails::buildMesh(util::CurveProvider &curveProvider) {
            const auto railsMeshData = train::res::MeshGenerator::rails(curveProvider);
            return train::gfx::Mesh::create(railsMeshData);
        }

        gfx::ShaderProgram &Rails::buildShader(gfx::ShaderManager &shaderManager) {
            return shaderManager.get(
                "vert/standard.glsl",
                "frag/standard.glsl",
                res::StandardVertex::Definition
            );
        }

        std::vector<glm::vec2> Rails::prepareShapePoints() {
            std::vector<glm::vec2> railsShapePoints = {
                glm::vec2(0.288303, 0.240586),
                glm::vec2(0.276757, 0.340764),
                glm::vec2(0.272813, 0.548107),
                glm::vec2(0.293575, 0.617194),
                glm::vec2(0.348842, 0.690956),
                glm::vec2(0.411263, 0.73931),
                glm::vec2(0.484306, 0.766086),
                glm::vec2(0.550069, 0.784728),
                glm::vec2(0.595381, 0.790535),
                glm::vec2(0.674657, 0.783739),
                glm::vec2(0.739287, 0.763628),
                glm::vec2(0.771424, 0.724455),
                glm::vec2(0.790399, 0.691322),
                glm::vec2(0.807127, 0.639125),
                glm::vec2(0.814835, 0.609894),
                glm::vec2(0.828993, 0.52728),
                glm::vec2(0.823483, 0.444588),
                glm::vec2(0.814181, 0.371234),
                glm::vec2(0.801099, 0.329199),
                glm::vec2(0.766606, 0.279841),
                glm::vec2(0.71984, 0.238458),
                glm::vec2(0.66224, 0.21283),
                glm::vec2(0.586086, 0.201889),
                glm::vec2(0.51724, 0.183282),
                glm::vec2(0.442815, 0.163004),
                glm::vec2(0.374567, 0.154858),
                glm::vec2(0.325035, 0.156444),
                glm::vec2(0.309431, 0.173424),
                glm::vec2(0.294111, 0.199631),
                glm::vec2(0.287147, 0.233636)
            };
            const glm::vec2 mergeDir = railsShapePoints[0] - railsShapePoints.back();
            railsShapePoints.push_back(railsShapePoints.back() + mergeDir * 0.9f);
            return move(railsShapePoints);
        }
    }
}