#include <functional>
#include <iostream>
#include "env/window.h"
#include "res/mesh-generator.h"
#include "gfx/mesh.h"
#include "gfx/shader-program.h"
#include "res/image-loader.h"
#include "gfx/texture.h"
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

    const int terrainWidth = 1024, terrainHeight = 1024;
    const auto gridData = train::res::MeshGenerator::Grid(terrainWidth, terrainHeight);
    auto grid = train::gfx::Mesh::create(gridData);
    auto shaderProgram = train::gfx::ShaderProgram::fromFile(
        "data/shaders/terrain.vert",
        "data/shaders/terrain.frag",
        gridData.vertexDefinition
    );

    auto terrainTexture = train::gfx::Texture::fromFile("data/textures/terrain_1.png");

    train::util::TimeStep realTimeTimeStep(train::util::TimeStep::Mode::Variable);
    realTimeTimeStep.addHandler(std::bind(&train::input::ICameraController::update, &cameraController, _1));
    realTimeTimeStep.start();

    glEnable(GL_DEPTH_TEST);
    while (mainWindow.exists()) {
        train::env::Window::processEvents();
        realTimeTimeStep.tick();

        glClearColor(0.0, 1.0f, 1.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.bind();
        shaderProgram.setUniform("projectionView", camera.getProjectionView());
        shaderProgram.setUniform("terrainSize", glm::vec3(terrainWidth, terrainHeight, 256.f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, terrainTexture.getHandle());
        shaderProgram.setUniform("terrainTexture", 0);

        grid.draw();
        shaderProgram.unbind();

        mainWindow.present();
    }

    return 0;
}