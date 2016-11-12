#include "time-step.h"
#include "../ogl.h"

namespace train {
    namespace util {
        TimeStep::TimeStep(Mode mode, double delta) :
            mode(mode), delta(delta) {
            reset();
        }

        void TimeStep::addHandler(StepFunction handler) {
            stepHandlers.push_back(std::move(handler));
        }

        void TimeStep::reset() {
            lastTime = glfwGetTime();
        }

        void TimeStep::stop() {
            updateAccumulator();
            reset();

            isStopped = true;
        }

        void TimeStep::start() {
            isStopped = false;
        }

        void TimeStep::tick() {
            updateAccumulator();

            switch (mode) {
                case Mode::Variable:
                    step(accumulator);
                    accumulator = 0.0;
                    break;

                case Mode::Fixed:
                    while (accumulator > delta) {
                        step(delta);
                        accumulator -= delta;
                    }

                    break;
            }
        }

        int TimeStep::getNextStepIndex() const {
            return nextStepIndex;
        }

        void TimeStep::updateAccumulator() {
            double currentTime = glfwGetTime();
            accumulator += currentTime - lastTime;
            lastTime = currentTime;
        }

        void TimeStep::step(double delta) {
            for (auto &f : stepHandlers)
                f(delta);
            ++nextStepIndex;
        }
    }
}