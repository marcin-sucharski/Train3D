#ifndef TRAIN_UTIL_CURVE_ITERATOR_H
#define TRAIN_UTIL_CURVE_ITERATOR_H

#include "curve-provider.h"

namespace train {
    namespace util {
        class CurveIterator {
            static const float baseStep;
            static const float limit;

            CurveProvider &curveProvider;
            CurvePoint currentPoint;
            float currentPosition;

        public:
            explicit CurveIterator(CurveProvider &curveProvider);

            const CurvePoint& getCurrentPoint() const;

            CurvePoint getNext(float distance);
            bool hasNext() const;

            void restart();
        };
    }
}

#endif // TRAIN_UTIL_CURVE_ITERATOR_H
