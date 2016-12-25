#include "image-curve-provider.h"

using namespace std;

namespace train {
    namespace res {
        ImageCurveProvider::ImageCurveProvider(const ImageData &imageData, std::vector<glm::vec2> shapePoints)
            : imageData(imageData), shapePoints(move(shapePoints)) {
        }

        util::CurvePoint train::res::ImageCurveProvider::getPointImpl(float t) {
            const float segmentLength = 1.0f / static_cast<float>(shapePoints.size());
            const int segmentIdx = static_cast<int>(glm::floor(t / segmentLength));



            return util::CurvePoint();
        }

        float ImageCurveProvider::getHeight(glm::vec2 pos) const {
            return 0;
        }
    }
}