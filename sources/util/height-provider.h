#ifndef TRAIN_UTIL_HEIGHT_PROVIDER_HPP
#define TRAIN_UTIL_HEIGHT_PROVIDER_HPP

#include <glm/glm.hpp>

namespace train {
    namespace util {
        struct HeightProvider {
            virtual ~HeightProvider() {}

            /*
             * Provides height value for normalized coordinates (both in (0.0, 1.0) ).
             */
            virtual float getHeight(glm::vec2 pos) = 0;

            glm::vec3 getNormal(glm::vec2 pos) {
                auto getPoint = [this, pos](glm::vec2 off) {
                    const glm::vec2 realPos(pos.x + off.x, pos.y + off.y);
                    return glm::vec3(realPos.x, getHeight(realPos), realPos.y);
                };
                auto has = [](glm::vec2 p) {
                    return p.x >= 0.0f && p.x <= 1.0f &&
                           p.y >= 0.0f && p.y <= 1.0f;
                };

                const glm::vec2 element = getElementSize();

                const glm::vec3 center = getPoint(pos);
                const glm::vec2 left(pos.x - element.x, pos.y);
                const glm::vec2 leftTop(pos.x - element.x, pos.y + element.y);
                const glm::vec2 right(pos.x + element.x, pos.y);
                const glm::vec2 rightBottom(pos.x + element.x, pos.y - element.y);
                const glm::vec2 down(pos.x, pos.y - element.y);
                const glm::vec2 up(pos.x, pos.y + element.y);

                glm::vec3 result;
                auto tryAdd = [&](glm::vec2 first, glm::vec2 second) {
                    if (has(first) && has(second)) {
                        auto p1 = getPoint(first), p2 = getPoint(second);
                        result += glm::cross(p1 - center, p2 - center);
                    }
                };

                tryAdd(left, leftTop);
                tryAdd(leftTop, up);
                tryAdd(up, right);
                tryAdd(right, rightBottom);
                tryAdd(rightBottom, down);
                tryAdd(down, left);

                return glm::normalize(result);
            }

        protected:
            virtual glm::vec2 getElementSize() = 0;
        };
    }
}

#endif // TRAIN_UTIL_HEIGHT_PROVIDER_HPP
