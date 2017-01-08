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
#include "obj/terrain.h"
#include "obj/rails.h"
#include "obj/train.h"

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

    train::obj::Terrain terrain(shaderManager);
    train::obj::Rails rails(shaderManager, terrain);
    train::obj::Train trainObject(shaderManager, rails);

    auto updateObjects = [&](double dt) {
        terrain.update(dt);
        rails.update(dt);
        trainObject.update(dt);
    };

    train::util::TimeStep realTimeTimeStep(train::util::TimeStep::Mode::Variable);
    realTimeTimeStep.addHandler(std::bind(&train::input::ICameraController::update, &cameraController, _1));
    realTimeTimeStep.addHandler(updateObjects);
    realTimeTimeStep.start();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);

    while (mainWindow.exists()) {
        train::env::Window::processEvents();
        realTimeTimeStep.tick();

        glClearColor(0.0, 1.0f, 1.0f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        terrain.draw(camera);
        rails.draw(camera);
        trainObject.draw(camera);

        mainWindow.present();

        if (glfwGetKey(mainWindow.getGLFWWindow(), GLFW_KEY_0) == GLFW_PRESS) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        if (glfwGetKey(mainWindow.getGLFWWindow(), GLFW_KEY_9) == GLFW_PRESS) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    }

    return 0;
}