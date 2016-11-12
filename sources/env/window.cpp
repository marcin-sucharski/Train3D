#include "window.h"

namespace train {
    namespace env {
        bool Window::s_IsGLFWInit = false;
        bool Window::s_IsGLEWInit = false;
        std::atomic<int> Window::s_WindowCount(0);

        namespace {
            const std::string selfDataName = "self";
        }

        Window::Window() : window(nullptr, glfwDestroyWindow) {
            ++s_WindowCount;
        }

        Window::Window(Window &&window) : Window() {
            *this = std::move(window);
        }

        Window::~Window() {
            --s_WindowCount;

            if (s_WindowCount == 0) {
                glfwMakeContextCurrent(nullptr);
                glfwTerminate();
                s_IsGLFWInit = false;
            }
        }

        Window& Window::operator =(Window &&window) {
            if (this != &window) {
                this->window = std::move(window.window);
                windowData = std::move(window.windowData);
            }

            return *this;
        }

        bool Window::init(int width, int height, const std::string &title, bool fullScreen) {
            if (!s_IsGLFWInit) {
                glfwInit();
                s_IsGLFWInit = true;
            }

            auto display = fullScreen ? glfwGetPrimaryMonitor() : nullptr;
            window.reset(glfwCreateWindow(width, height, title.c_str(), display, nullptr));

            if (!window) {
                return false;
            }

            glfwSetWindowUserPointer(window.get(), &windowData);
            glfwSetWindowSizeCallback(window.get(), &Window::handleResize);
            setWindowData(getGLFWWindow(), selfDataName, this);
            makeContextCurrent();

            if (!s_IsGLEWInit) {
                glewExperimental = GL_TRUE;
                glewInit();
                s_IsGLEWInit = true;
            }
            glViewport(0, 0, width, height);
            return true;
        }

        void Window::dispose() {
            window = nullptr;
            windowData.clear();
        }

        void Window::present() {
            glfwSwapBuffers(window.get());
        }

        void Window::makeContextCurrent() {
            glfwMakeContextCurrent(window.get());
        }

        void Window::processEvents() {
            glfwPollEvents();
        }

        bool Window::exists() {
            if (glfwWindowShouldClose(window.get())) {
                dispose();
            }
            return window != nullptr;
        }

        GLFWwindow *Window::getGLFWWindow() const {
            return window.get();
        }

        void Window::setResizeCallback(std::function<void(int, int)> onResize) {
            this->onResize = std::move(onResize);
        }

        void Window::handleResize(GLFWwindow *window, int width, int height) {
            Window* self = getWindowData<Window>(window, selfDataName);
            self->makeContextCurrent();
            glViewport(0, 0, width, height);
            if (self->onResize) {
                self->onResize(width, height);
            }
        }
    }
}