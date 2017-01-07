#ifndef TRAIN_OBJ_RAILS_H
#define TRAIN_OBJ_RAILS_H

#include "object.h"
#include "../gfx/shader-manager.h"
#include "terrain.h"
#include <memory>
#include "../util/curve-provider.h"

namespace train {
    namespace obj {
        class Rails : public Object {

            std::unique_ptr<util::CurveProvider> curveProvider;
            gfx::Mesh mesh;
            gfx::ShaderProgram &shader;
            gfx::Texture texture;

        public:
            Rails(
                gfx::ShaderManager &shaderManager,
                Terrain &terrain
            );

            void update(double dt) override;
            void draw(const model::Camera &camera) override;

        private:
            static gfx::Mesh buildMesh(util::CurveProvider &curveProvider);
            static gfx::ShaderProgram &buildShader(gfx::ShaderManager &shaderManager);

            static std::vector<glm::vec2> prepareShapePoints();
        };
    }
}

#endif // TRAIN_OBJ_RAILS_H
