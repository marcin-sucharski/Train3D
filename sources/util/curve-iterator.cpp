#include "curve-iterator.h"

using namespace glm;

namespace train {
    namespace util {
        const float CurveIterator::baseStep = 0.005f;
        const float CurveIterator::limit = 0.001f;

        CurveIterator::CurveIterator(CurveProvider &curveProvider)
            : curveProvider(curveProvider) {
            restart();
        }

        const CurvePoint& CurveIterator::getCurrentPoint() const {
            return currentPoint;
        }

        CurvePoint CurveIterator::getNext(float distance) {
            float step = baseStep;
            float position = currentPosition, prevPosition;

            util::CurvePoint nextPoint;
            bool result;
            do {
                prevPosition = position;
                position += step;
                position = clamp(position, 0.0f, 1.0f);

                nextPoint = curveProvider.getPoint(position);
                const float dis = glm::distance(currentPoint.position, nextPoint.position);
                result = abs(dis - distance) >= limit;

                if (dis > distance) {
                    position = prevPosition;
                    step *= 0.5f;
                }

                if (dis < distance && position == 1.0f) {
                    break;
                }
            } while (result);

            currentPoint = nextPoint;
            currentPosition = position;
            return nextPoint;
        }

        bool CurveIterator::hasNext() const {
            return abs(currentPosition - 1.0f) > limit;
        }

        void CurveIterator::restart() {
            currentPosition = 0.0f;
            currentPoint = curveProvider.getPoint(currentPosition);
        }
    }
}
