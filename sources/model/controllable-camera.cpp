#include "controllable-camera.h"

namespace train {
    namespace model {
        void ControllableCamera::update(const glm::vec3 &position, const glm::vec3 &forward) {
            this->position = position;
            this->forward = forward;
            updateImpl();
        }

        const glm::vec3 &ControllableCamera::getPosition() const {
            return position;
        }

        const glm::vec3 &ControllableCamera::getForwardVector() const {
            return forward;
        }
    }
}