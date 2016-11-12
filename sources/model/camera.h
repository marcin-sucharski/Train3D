#ifndef TRAIN_MODEL_CAMERA_H
#define TRAIN_MODEL_CAMERA_H

#include <glm/glm.hpp>
#include "controllable-camera.h"

namespace train {
    namespace model {
        class Camera : public ControllableCamera {
            glm::mat4x4 projection;
            glm::mat4x4 view;
            glm::mat4x4 projectionView;

        public:
            void setupProjection(float viewWidth, float viewHeight);

            const glm::mat4x4 &getProjection() const;
            const glm::mat4x4 &getView() const;
            const glm::mat4x4 &getProjectionView() const;

        protected:
            virtual void updateImpl() override;

            void updateProjectionView();
        };
    }
}


#endif
