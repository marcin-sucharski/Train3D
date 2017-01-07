#include "image-curve-provider.h"
#include <iostream>

using namespace std;
using namespace glm;

namespace train {
    namespace res {
        ImageCurveProvider::ImageCurveProvider(
            util::HeightProvider& heightProvider,
            glm::vec3 terrainScale,
            std::vector<vec2> shapePoints
        ) : heightProvider(heightProvider), terrainScale(terrainScale), shapePoints(move(shapePoints)) {
        }

        util::CurvePoint train::res::ImageCurveProvider::getPointImpl(float t) {
            const float diff = 0.01;
            const auto curr = getImagePos(t);
            const auto next = getImagePos(t + diff);

            const auto currPos = vec3(curr.x, heightProvider.getHeight(curr), curr.y);
            const auto nextPos = vec3(next.x, heightProvider.getHeight(next), next.y);

            const auto forward = normalize(nextPos - currPos);

            const auto off = vec3(0.01f, 0.01f, 0.0);
            const float hL = heightProvider.getHeight(curr - vec2(off.x, off.z));
            const float hR = heightProvider.getHeight(curr + vec2(off.x, off.z));
            const float hD = heightProvider.getHeight(curr - vec2(off.z, off.y));
            const float hU = heightProvider.getHeight(curr + vec2(off.z, off.y));

            const auto up = normalize(vec3(hL - hR, hD - hU, 0.0012f));
            const auto pos = vec3(currPos.x * terrainScale.x, currPos.y, currPos.z * terrainScale.y);

            cout << "t=" << t << "; pos=vec3(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << endl;
            return util::CurvePoint(pos, up, forward);
        }

        vec2 ImageCurveProvider::getImagePos(float t) const {
            const float segmentLength = 1.0f / static_cast<float>(shapePoints.size() - 1);
            const float basePos = floor(t / segmentLength);
            const size_t segmentFirstIdx = static_cast<size_t>(basePos);
            const size_t segmentSecondIdx = static_cast<size_t>(ceil(t / segmentLength));

            const vec2 currentPoint = shapePoints[segmentFirstIdx];
            const vec2 nextPoint = shapePoints[segmentSecondIdx];

            const float interpolation = (t / segmentLength) - basePos;
            return currentPoint * (1.0f - interpolation) + nextPoint * interpolation;
        }
    }
}