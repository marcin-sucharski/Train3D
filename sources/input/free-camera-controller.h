#ifndef TRAIN_INPUT_FREE_CAMERA_CONTROLLER_H
#define TRAIN_INPUT_FREE_CAMERA_CONTROLLER_H

#include "icamera-controller.h"
#include <glm/glm.hpp>

namespace train {
    namespace env {
        class Window;
    }
    namespace model {
        class ControllableCamera;
    }

    namespace input {
        class FreeCameraController : public ICameraController {
            static const glm::vec2 neutralMousePos;

            model::ControllableCamera &camera;
            env::Window &window;

            float speed;

        public:
            FreeCameraController(model::ControllableCamera &camera, env::Window &window);

            virtual void setup() override;
            virtual void free() override;

            virtual void update(double delta) override;

        private:
            glm::vec2 getCurrentMousePos() const;

            bool isForwardPressed() const;
            bool isBackwardPressed() const;
            bool isLeftPressed() const;
            bool isRightPressed() const;

            bool checkKey(int keyCode) const;

            void setMousePos();
        };
    }
}

#endif
