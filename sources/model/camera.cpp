#include "camera.h"
#include "../ogl.h"

namespace train {
    namespace model {
        void Camera::setupProjection(float viewWidth, float viewHeight) {
            const float defaultFov = 75.0f;
            const float defaultZNear = 0.1f;
            const float defaultZFar = 128.0f;

            projection = glm::perspective(
                glm::radians(defaultFov),
                viewWidth / viewHeight,
                defaultZNear,
                defaultZFar
            );
            updateProjectionView();
        }

        const glm::mat4x4 &Camera::getProjection() const {
            return projection;
        }

        const glm::mat4x4 &Camera::getView() const {
            return view;
        }

        const glm::mat4x4 &Camera::getProjectionView() const {
            return projectionView;
        }

        void Camera::updateImpl() {
            view = glm::lookAt(
                position,
                position + forward,
                glm::vec3(0.f, 1.0f, 0.f)
            );
            updateProjectionView();
        }

        void Camera::updateProjectionView() {
            projectionView = projection * view;
        }
    }
}