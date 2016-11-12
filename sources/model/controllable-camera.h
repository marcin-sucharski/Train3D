#ifndef TRAIN_MODEL_CONTROLLABLE_CAMERA_H
#define TRAIN_MODEL_CONTROLLABLE_CAMERA_H

#include <glm/glm.hpp>

namespace train {
    namespace model {
        struct ControllableCamera {
            virtual ~ControllableCamera() {}

            void update(const glm::vec3 &position, const glm::vec3 &forward);

            const glm::vec3 &getPosition() const;
            const glm::vec3 &getForwardVector() const;

        protected:
            glm::vec3 position;
            glm::vec3 forward;

            virtual void updateImpl() = 0;
        };
    }
}

#endif
