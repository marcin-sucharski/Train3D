#ifndef TRAIN_ENV_WINDOW_H
#define TRAIN_ENV_WINDOW_H

#include <cassert>
#include <atomic>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <memory>

#include "../ogl.h"

namespace train {
    namespace env {
        namespace detail {
            typedef std::unordered_map<std::string, void*> WindowUserData;
            inline WindowUserData& getWindowUserData(GLFWwindow *window) {
                assert(window);
                return *static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));
            }
        }

        /// Sets user data with specifed key.
        inline void setWindowData(GLFWwindow *window, const std::string &key, void *userData) {
            detail::getWindowUserData(window)[key] = userData;
        }

        /// Returns user data associated with specified window with given key.
        template <typename T>
        inline T* getWindowData(GLFWwindow *window, const std::string &key) {
            return static_cast<T*>(detail::getWindowUserData(window)[key]);
        }

        class Window {
            typedef std::add_pointer<decltype(glfwDestroyWindow)>::type WindowDeleter;
            typedef std::unique_ptr<GLFWwindow, WindowDeleter> WinPtr;

            static bool s_IsGLFWInit;
            static bool s_IsGLEWInit;
            static std::atomic<int> s_WindowCount;

            WinPtr window;
            detail::WindowUserData windowData;

            std::function<void(int, int)> onResize;

        public:
            Window();
            Window(const Window&) = delete;
            Window(Window &&window);
            ~Window();

            Window& operator =(const Window&) = delete;
            Window& operator =(Window &&window);

            /// Initializes window and OpenGL context.
            bool init(int width, int height, const std::string &title, bool fullScreen);

            void setResizeCallback(std::function<void(int, int)> onResize);

            /// Releases resources associated with window.
            void dispose();

            /// Presents opengl's back buffer onto screen
            void present();

            /// Sets OpenGL context associated with window as active.
            void makeContextCurrent();

            bool exists();

            GLFWwindow* getGLFWWindow() const;

            /// Processes messages from OS for all windows
            ///
            /// @remarks Should be called on every frame.
            static void processEvents();

        private:
            static void handleResize(GLFWwindow *window, int width, int height);
        };
    }
}

#endif
