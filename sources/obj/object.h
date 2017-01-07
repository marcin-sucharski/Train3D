#ifndef TRAIN_OBJ_OBJECT_H
#define TRAIN_OBJ_OBJECT_H

#include "../model/camera.h"

namespace train {
    namespace obj {
        struct Object {
            virtual ~Object() {}

            virtual void update(double dt) = 0;
            virtual void draw(const model::Camera &camera) = 0;
        };
    }
}

#endif // TRAIN_OBJ_OBJECT_H
