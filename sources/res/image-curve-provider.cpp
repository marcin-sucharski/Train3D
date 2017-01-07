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
            const float diff = 0.0001;
            const bool fakeNext = (t + diff) > 1.0f;

            const auto curr = getImagePos(t), next = getImagePos(fakeNext ? t - diff : t + diff);

            const auto currPos = vec3(curr.x, heightProvider.getHeight(curr), curr.y);
            const auto nextPos = vec3(next.x, heightProvider.getHeight(next), next.y);
            const auto forward = normalize(fakeNext ? currPos - nextPos : nextPos - currPos);

            const auto up = heightProvider.getNormal(curr, vec3(terrainScale.x, terrainScale.z, terrainScale.y));
            const auto pos = vec3(
                currPos.x * (terrainScale.x + 1),
                currPos.y * terrainScale.z,
                currPos.z * (terrainScale.y + 1)
            );

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