#ifndef TRAIN_UTIL_TIME_STEP_H
#define TRAIN_UTIL_TIME_STEP_H

#include <vector>
#include <functional>

namespace train {
    namespace util {
        class TimeStep {
        public:
            enum class Mode {
                /// Step function is called on every tick.
                ///
                /// @remarks Delta time is variable.
                Variable,

                /// Step function is called when time accumulator is greater than delta.
                ///
                /// @remarks Delta time is constant.
                Fixed,
            };

            // void StepFunction(double delta)
            typedef std::function<void(double)> StepFunction;

        private:
            const Mode mode;
            const double delta;

            double accumulator = 0.0;
            double lastTime;
            bool isStopped = false;

            std::vector<StepFunction> stepHandlers;
            int nextStepIndex = 0;

        public:
            TimeStep(Mode mode, double delta = 1.0 / 30.0);
            TimeStep(const TimeStep &) = delete;
            TimeStep(TimeStep &&) = delete;

            TimeStep &operator=(const TimeStep &) = delete;
            TimeStep &operator=(TimeStep &&) = delete;

            ///	Adds step handler.
            ///
            /// @param stepFunction Handler for step.
            void addHandler(StepFunction stepFunction);

            /// Resets internal timer.
            void reset();

            ///	Stops internal timer.
            ///
            /// @remarks Has to be called when pause in Tick
            /// calls shouldn't be counted.
            void stop();

            /// Starts internal timer.
            ///
            /// @remarks Has to be called after Stop call.
            void start();

            /// Updates timer. Calls step functions if needed.
            void tick();

            /// @return Index of next step (starts from 0).
            /// @remarks Index is increased after each step.
            int getNextStepIndex() const;

        private:
            void updateAccumulator();

            void step(double delta);
        };
    }
}

#endif
