#include "train.h"
#include "../res/vertex-format.h"
#include "../res/mesh-generator.h"
#include <glm/gtx/vector_angle.hpp>

using namespace glm;

namespace train {
    namespace obj {
        const float Train::initialVelocity = 7.f;
        const float Train::minimalVelocity = 2.f;
        const float Train::plankLength = 1.5f;
        const float Train::plankWidth = 0.64f;
        const float Train::plankHeight = 0.1f;
        const float Train::wheelRadius = 0.4f;
        const float Train::wheelWidth = 0.07f;
        const float Train::connectionWidth = 0.06f;
        const float Train::trainYOffset = wheelRadius + 0.1f;
        const float Train::accelerationG = 9.8f;

        Train::Train(gfx::ShaderManager &shaderManager, Rails &rails)
            : curveProvider(rails.getCurve()),
              curve(rails.getCurve()),
              wheelAngle(0.0f),
              velocity(initialVelocity),
              totalDistance(0.f),
              wheelTexture(gfx::Texture::fromFile("data/textures/wheel.png")),
              plankTexture(gfx::Texture::fromFile("data/textures/plank.png")),
              wheelMesh(buildWheelMesh()),
              plankMesh(buildPlankMesh()),
              wheelConnectionMesh(buildWheelConnectionMesh()),
              shader(prepareShader(shaderManager)) {
        }

        void Train::update(double dt) {
            if (!curve.hasNext()) {
                curve.restart();
            }

            const auto curr = curve.getCurrentPoint();
            const auto diff = static_cast<float>(dt) * velocity;
            const auto next = curve.getNext(diff);
            totalDistance += diff;

            const float kineticEnergy = velocity * velocity * 0.5f;
            const float energyDiff = (curr.position.y - next.position.y) * accelerationG;
            const float newKineticEnergy = kineticEnergy + energyDiff;

            velocity = sqrt(newKineticEnergy * 2.f);
            velocity = max(velocity, minimalVelocity);
            velocity -= (velocity * velocity) * 0.01f * dt;
        }

        void Train::draw(const model::Camera &camera) {
            const auto current = curve.getCurrentPoint();

            mat4x4 trainModel = inverse(lookAt(
                current.position,
                current.position + current.forward,
                current.up
            ));
            trainModel = rotate(trainModel, radians(90.f), vec3(0.f, 1.f, 0.f));
            trainModel = translate(trainModel, vec3(0.f, trainYOffset, 0.f));

            shader.bind();
            shader.setUniform("projectionView", camera.getProjectionView());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wheelTexture.getHandle());
            shader.setUniform("diffuse", 0);

            const float sign[2] = {1.0f, -1.0f};
            for (int i = 0; i < 4; ++i) {
                mat4x4 model = trainModel;

                model = translate(model, vec3(0.f, 0.f, 1.f) * plankWidth * 0.5f * sign[(i + 1) % 2]);
                model = translate(model, vec3(0.f, 0.f, 1.f) * wheelWidth * 0.5f * sign[(i + 1) % 2]);
                model = translate(model, vec3(1.f, 0.f, 0.f) * plankLength * 0.5f * 0.9f * sign[i >= 2]);
                model = rotate(model, radians(90.f), vec3(1.f, 0.f, 0.f));
                model = translate(model, vec3(0.f, wheelWidth * -0.5f, 0.f));
                const float rot = totalDistance / wheelRadius;
                model = rotate(model, pi<float>() * 0.3f * i - rot, vec3(0.f, 1.f, 0.f));

                shader.setUniform("model", model);
                shader.setUniform("invModel", transpose(inverse(model)));
                wheelMesh.draw();
            }
            for (int i = 0; i < 2; ++i) {
                mat4x4 model = trainModel;

                model = translate(model, vec3(0.f, 0.f, plankWidth * 0.5f + wheelWidth * 1.5f) * sign[i % 2]);
                const float rot = totalDistance / wheelRadius;
                model = translate(model, vec3(sin(rot), cos(rot), 0.f) * wheelRadius * 0.8f);

                shader.setUniform("model", model);
                shader.setUniform("invModel", transpose(inverse(model)));
                wheelConnectionMesh.draw();
            }

            glBindTexture(GL_TEXTURE_2D, wheelTexture.getHandle());

            mat4x4 plankModel = trainModel;

            shader.setUniform("model", plankModel);
            shader.setUniform("invModel", transpose(inverse(plankModel)));
            plankMesh.draw();

            shader.unbind();
        }

        gfx::ShaderProgram &Train::prepareShader(gfx::ShaderManager &shaderManager) {
            return shaderManager.get(
                "vert/standard.glsl",
                "frag/standard.glsl",
                res::StandardVertex::Definition
            );
        }

        gfx::Mesh Train::buildWheelMesh() {
            auto wheelMeshData = res::MeshGenerator::cylinder(64, wheelRadius, wheelWidth);
            return gfx::Mesh::create(wheelMeshData);
        }

        gfx::Mesh Train::buildPlankMesh() {
            auto plankMeshData = res::MeshGenerator::texturedCube(vec3(plankLength, plankHeight, plankWidth) * 0.5f);
            return gfx::Mesh::create(plankMeshData);
        }

        gfx::Mesh Train::buildWheelConnectionMesh() {
            auto conn = res::MeshGenerator::texturedCube(vec3(
                plankLength * 0.5f * 0.9f,
                connectionWidth,
                connectionWidth
            ));
            return gfx::Mesh::create(conn);
        }
    }
}