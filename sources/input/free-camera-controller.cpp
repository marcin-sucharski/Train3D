#include <math.h>
#include "free-camera-controller.h"
#include "../model/controllable-camera.h"
#include "../env/window.h"
#include "../ogl.h"

namespace train {
    namespace input {
        const glm::vec2 FreeCameraController::neutralMousePos = glm::vec2(400.f, 400.f);

        FreeCameraController::FreeCameraController(model::ControllableCamera &camera, env::Window &window)
            : camera(camera), window(window) {
            setup();
        }

        void FreeCameraController::setup() {
            glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            setMousePos();
        }

        void FreeCameraController::free() {
            glfwSetInputMode(window.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        void FreeCameraController::update(double delta) {
            const float cameraMoveSpeed = 0.6f * 60.f * static_cast<float>(delta);
            const float cameraRotateSpeed = 0.0015f;
            const glm::vec3 forward = camera.getForwardVector();
            const glm::vec3 leftVec = glm::normalize(glm::cross(forward, glm::vec3(0.f, 1.f, 0.f)));
            const glm::vec3 tangentUp = glm::normalize(glm::cross(forward, leftVec));
            const glm::vec3 position = camera.getPosition();
            const glm::vec2 mousePos = getCurrentMousePos();

            const glm::vec2 dMove = (mousePos - neutralMousePos) * cameraRotateSpeed;
            const glm::vec3 newForward = glm::normalize(forward + leftVec * dMove.x + tangentUp * dMove.y);

            const glm::vec3 moveForward = isForwardPressed() ? forward * cameraMoveSpeed : glm::vec3();
            const glm::vec3 moveBackward = isBackwardPressed() ? forward * -cameraMoveSpeed : glm::vec3();

            const glm::vec3 moveLeft = isLeftPressed() ? leftVec * -cameraMoveSpeed : glm::vec3();
            const glm::vec3 moveRight = isRightPressed() ? leftVec * cameraMoveSpeed : glm::vec3();

            const glm::vec3 newPosition = position + moveForward + moveBackward + moveLeft + moveRight;

            camera.update(newPosition, newForward);
            setMousePos();
        }

        glm::vec2 FreeCameraController::getCurrentMousePos() const {
            glm::dvec2 mousePos;
            glfwGetCursorPos(window.getGLFWWindow(), &mousePos.x, &mousePos.y);
            return mousePos;
        }

        bool FreeCameraController::isForwardPressed() const {
            return checkKey(GLFW_KEY_W);
        }

        bool FreeCameraController::isBackwardPressed() const {
            return checkKey(GLFW_KEY_S);
        }

        bool FreeCameraController::isLeftPressed() const {
            return checkKey(GLFW_KEY_A);
        }

        bool FreeCameraController::isRightPressed() const {
            return checkKey(GLFW_KEY_D);
        }

        bool FreeCameraController::checkKey(int keyCode) const {
            return glfwGetKey(window.getGLFWWindow(), keyCode) == GLFW_PRESS;
        }

        void FreeCameraController::setMousePos() {
            glfwSetCursorPos(window.getGLFWWindow(), neutralMousePos.x, neutralMousePos.y);
        }
    }
}