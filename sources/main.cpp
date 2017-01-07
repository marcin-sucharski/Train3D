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
#include "res/image-curve-provider.h"
#include "res/image-height-provider.h"
#include "env/file.h"

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
    const auto gridData = train::res::MeshGenerator::grid(terrainWidth, terrainHeight);
    auto grid = train::gfx::Mesh::create(gridData);
    auto &shaderProgram = shaderManager.get(
        "vert/terrain.glsl",
        "frag/terrain.glsl",
        gridData.vertexDefinition
    );

    const auto lightCubeData = train::res::MeshGenerator::coloredCube();
    auto lightCube = train::gfx::Mesh::create(lightCubeData);
    auto &cubeShaderProgram = shaderManager.get(
        "vert/simple-transform.glsl",
        "frag/simple.glsl",
        lightCubeData.vertexDefinition
    );

    train::env::File terrainTextureFile("data/textures/terrain_2.png");
    train::res::ImageData terrainTextureData = train::res::ImageLoader::load(terrainTextureFile);
    const auto terrainScale = glm::vec3(terrainWidth, terrainHeight, 48.f);
    train::res::ImageHeightProvider heightProvider(terrainTextureData);

    auto terrainMeshData = train::res::MeshGenerator::terrain(
        terrainWidth, terrainHeight,
        heightProvider,
        terrainScale.z
    );
    auto terrainMesh = train::gfx::Mesh::create(terrainMeshData);
    auto grassTexture = train::gfx::Texture::fromFile("data/textures/grass_grass_0124_01_tiled_s.jpg");
    auto rockTexture = train::gfx::Texture::fromFile("data/textures/ground_stone_ground_0002_02_tiled_s.jpg");

    auto railsCurve = train::res::ImageCurveProvider(heightProvider, terrainScale, {
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
        glm::vec2(0.287147, 0.233636),
        glm::vec2(0.288303, 0.240586)
    });
    const auto railsMeshData = train::res::MeshGenerator::rails(railsCurve);
    auto rails = train::gfx::Mesh::create(railsMeshData);

    train::util::TimeStep realTimeTimeStep(train::util::TimeStep::Mode::Variable);
    realTimeTimeStep.addHandler(std::bind(&train::input::ICameraController::update, &cameraController, _1));
    realTimeTimeStep.start();

    float normalCoefficient = 0.0012f;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);

    while (mainWindow.exists()) {
        train::env::Window::processEvents();
        realTimeTimeStep.tick();

        glClearColor(0.0, 1.0f, 1.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.bind();
        shaderProgram.setUniform("projectionView", camera.getProjectionView());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture.getHandle());
        shaderProgram.setUniform("textureGrass", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rockTexture.getHandle());
        shaderProgram.setUniform("textureRock", 1);
        terrainMesh.draw();
        shaderProgram.unbind();

        cubeShaderProgram.bind();
        cubeShaderProgram.setUniform("projectionView", camera.getProjectionView());
        cubeShaderProgram.setUniform("model", glm::mat4x4());
        rails.draw();
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