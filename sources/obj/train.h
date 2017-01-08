#ifndef TRAIN_OBJ_TRAIN_H
#define TRAIN_OBJ_TRAIN_H

#include "object.h"
#include "rails.h"
#include "../util/curve-provider.h"
#include "../util/curve-iterator.h"
#include "../gfx/shader-manager.h"

namespace train {
    namespace obj {
        class Train : public Object {
            static const float initialVelocity;
            static const float minimalVelocity;

            static const float plankLength;
            static const float plankWidth;
            static const float plankHeight;
            static const float wheelRadius;
            static const float wheelWidth;
            static const float connectionWidth;

            static const float trainYOffset;

            static const float accelerationG;

            util::CurveProvider& curveProvider;
            util::CurveIterator curve;

            float wheelAngle;
            float velocity;

            float totalDistance;

            gfx::Texture wheelTexture;
            gfx::Texture plankTexture;

            gfx::Mesh wheelMesh;
            gfx::Mesh plankMesh;
            gfx::Mesh wheelConnectionMesh;

            gfx::ShaderProgram &shader;

        public:
            Train(
                gfx::ShaderManager &shaderManager,
                Rails &rails
            );

            void update(double dt) override;
            void draw(const model::Camera &camera) override;

        private:
            static gfx::ShaderProgram &prepareShader(gfx::ShaderManager &shaderManager);
            static gfx::Mesh buildWheelMesh();
            static gfx::Mesh buildPlankMesh();
            static gfx::Mesh buildWheelConnectionMesh();
        };
    }
}

#endif // TRAIN_OBJ_TRAIN_H
