#include <functional>
#include <iostream>
#include "env/window.h"
#include "res/mesh-generator.h"
#include "gfx/mesh.h"
#include "gfx/shader-program.h"
#include "model/camera.h"
#include "input/free-camera-controller.h"
#include "util/time-step.h"

using namespace std::placeholders;

const int initialWindowWidth = 800;
const int initialWindowHeight = 600;

int main() {
    train::env::Window mainWindow;
    mainWindow.init(initialWindowWidth, initialWindowHeight, "Train", false);

    train::model::Camera camera;
    camera.setupProjection(static_cast<float>(initialWindowWidth), static_cast<float>(initialWindowHeight));
    mainWindow.setResizeCallback(std::bind(&train::model::Camera::setupProjection, camera, _1, _2));
    camera.update(glm::vec3(0.f, 0.f, -5.f), glm::vec3(0.f, 0.f, 1.f));
    train::input::FreeCameraController cameraController(camera, mainWindow);

    const auto &triangleData = train::res::MeshGenerator::SampleTriangle();
    auto triangle = train::gfx::Mesh::create(triangleData);
    auto shaderProgram = train::gfx::ShaderProgram::fromFile(
        "data/shaders/simple-transform.vert",
        "data/shaders/simple.frag",
        triangleData.vertexDefinition
    );

    train::util::TimeStep realTimeTimeStep(train::util::TimeStep::Mode::Variable);
    realTimeTimeStep.addHandler(std::bind(&train::input::ICameraController::update, &cameraController, _1));
    realTimeTimeStep.start();
    while (mainWindow.exists()) {
        train::env::Window::processEvents();
        realTimeTimeStep.tick();

        glClearColor(0.0, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.bind();
        shaderProgram.setUniform("projectionView", camera.getProjectionView());
        triangle.draw();
        shaderProgram.unbind();

        mainWindow.present();
    }

    return 0;
}