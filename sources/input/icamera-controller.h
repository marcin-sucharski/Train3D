#ifndef TRAIN_INPUT_ICAMERA_CONTROLLER_H
#define TRAIN_INPUT_ICAMERA_CONTROLLER_H

namespace train {
    namespace input {
        struct ICameraController {
            virtual ~ICameraController() {}

            virtual void setup() = 0;
            virtual void free() = 0;

            virtual void update(double delta) = 0;
        };
    }
}

#endif
