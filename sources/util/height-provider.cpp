#include "height-provider.h"
#include <iostream>

using namespace glm;

namespace train {
    namespace util {
        vec3 HeightProvider::getNormal(vec2 pos) {
            auto getPoint = [this](vec2 realPos) { return vec3(realPos.x, getHeight(realPos), realPos.y); };
            auto has = [](vec2 p) { return p.x >= 0.0f && p.x <= 1.0f && p.y >= 0.0f && p.y <= 1.0f; };

            const vec2 element = getElementSize();

            const vec3 center = getPoint(pos);
            const vec2 left(pos.x - element.x, pos.y);
            const vec2 leftTop(pos.x - element.x, pos.y + element.y);
            const vec2 right(pos.x + element.x, pos.y);
            const vec2 rightBottom(pos.x + element.x, pos.y - element.y);
            const vec2 down(pos.x, pos.y - element.y);
            const vec2 up(pos.x, pos.y + element.y);

            vec3 result;
            auto tryAdd = [&](vec2 first, vec2 second) {
                if (has(first) && has(second)) {
                    auto p1 = getPoint(first), p2 = getPoint(second);
                    result += normalize(cross(p1 - center, p2 - center));
                }
            };

            tryAdd(left, leftTop);
            tryAdd(leftTop, up);
            tryAdd(up, right);
            tryAdd(right, rightBottom);
            tryAdd(rightBottom, down);
            tryAdd(down, left);

            return normalize(result);
        }
    }
}