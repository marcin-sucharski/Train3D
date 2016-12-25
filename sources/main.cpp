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
#include "gfx/shader-manager.h"
#include "util/circle-curve-provider.h"

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

    train::gfx::ShaderManager shaderManager;

    const int terrainWidth = 256, terrainHeight = 256;
    const auto gridData = train::res::MeshGenerator::Grid(terrainWidth, terrainHeight);
    auto grid = train::gfx::Mesh::create(gridData);
    auto &shaderProgram = shaderManager.get(
        "vert/terrain.glsl",
        "frag/terrain.glsl",
        gridData.vertexDefinition
    );
    auto terrainTexture = train::gfx::Texture::fromFile("data/textures/terrain_1.png");

    const auto lightCubeData = train::res::MeshGenerator::ColoredCube();
    auto lightCube = train::gfx::Mesh::create(lightCubeData);
    auto &cubeShaderProgram = shaderManager.get(
        "vert/simple-transform.glsl",
        "frag/simple.glsl",
        lightCubeData.vertexDefinition
    );

    auto circle = train::util::CircleCurveProvider(glm::vec3(0.f, 45.f, 0.f), 40.0f);
    const auto circleRailsData= train::res::MeshGenerator::rails(circle);
    auto circleRails = train::gfx::Mesh::create(circleRailsData);

    train::util::TimeStep realTimeTimeStep(train::util::TimeStep::Mode::Variable);
    realTimeTimeStep.addHandler(std::bind(&train::input::ICameraController::update, &cameraController, _1));
    realTimeTimeStep.start();

    float normalCoefficient = 0.0092f;
    glm::vec3 lightPos(0.0f, 128.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);
    while (mainWindow.exists()) {
        train::env::Window::processEvents();
        realTimeTimeStep.tick();

        lightPos.x = std::sin(static_cast<float>(glfwGetTime() / 3.)) * 128.0f;
        lightPos.z = std::cos(static_cast<float>(glfwGetTime() / 3.)) * 128.0f;

        glClearColor(0.0, 1.0f, 1.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.bind();
        shaderProgram.setUniform("projectionView", camera.getProjectionView());
        shaderProgram.setUniform("terrainSize", glm::vec3(terrainWidth, terrainHeight, 64.f));
        shaderProgram.setUniform("normalCoefficient", normalCoefficient);
        shaderProgram.setUniform("lightPos", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, terrainTexture.getHandle());
        shaderProgram.setUniform("terrainTexture", 0);

        grid.draw();
        shaderProgram.unbind();

        cubeShaderProgram.bind();
        cubeShaderProgram.setUniform("projectionView", camera.getProjectionView());
        cubeShaderProgram.setUniform("model", glm::translate(glm::mat4x4(), lightPos));
        lightCube.draw();
        cubeShaderProgram.setUniform("model", glm::mat4x4());
        circleRails.draw();
        cubeShaderProgram.unbind();

        mainWindow.present();

        if (glfwGetKey(mainWindow.getGLFWWindow(), GLFW_KEY_0) == GLFW_PRESS) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        if (glfwGetKey(mainWindow.getGLFWWindow(), GLFW_KEY_9) == GLFW_PRESS) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        if (glfwGetKey(mainWindow.getGLFWWindow(), GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
            normalCoefficient *= 1.01;
        }
        if (glfwGetKey(mainWindow.getGLFWWindow(), GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
            normalCoefficient *= 0.99;
        }
        if (glfwGetKey(mainWindow.getGLFWWindow(), GLFW_KEY_SLASH) == GLFW_PRESS) {
            std::cout << normalCoefficient << std::endl;
        }
    }

    return 0;
}