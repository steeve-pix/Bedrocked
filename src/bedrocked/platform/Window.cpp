#include "bedrocked/platform/Window.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace {
    [[nodiscard]] int toGlfwKey(bedrocked::Key key) noexcept {
        switch (key) {
            case bedrocked::Key::Escape: return GLFW_KEY_ESCAPE;
            case bedrocked::Key::W: return GLFW_KEY_W;
            case bedrocked::Key::A: return GLFW_KEY_A;
            case bedrocked::Key::S: return GLFW_KEY_S;
            case bedrocked::Key::D: return GLFW_KEY_D;
            case bedrocked::Key::Space: return GLFW_KEY_SPACE;
            case bedrocked::Key::LeftShift: return GLFW_KEY_LEFT_SHIFT;
        }
        return GLFW_KEY_UNKNOWN;
    }
} // Anonymouse helper

namespace bedrocked {
    Window::Window(const WindowConfig &config) {
        // Initialize GLFW
        if (!glfwInit()) {
            throw std::runtime_error{"Failed to initialize GLFW"};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create the window
        m_handle = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);

        if (!m_handle) {
            glfwTerminate();
            throw std::runtime_error{"Failed to create GLFW window"};
        }

        // Bind the OpenGL context to current threaad
        glfwMakeContextCurrent(m_handle);
    }

    Window::~Window() {
        if (m_handle) {
            glfwDestroyWindow(m_handle);
        }
        glfwTerminate();
    }

    bool Window::shouldClose() const noexcept {
        return glfwWindowShouldClose(m_handle);
    }

    void Window::pollEvents() noexcept {
        glfwPollEvents();
    }

    bool Window::isKeyDown(Key key) const noexcept {
        const int glfwKey = toGlfwKey(key);
        if (glfwKey == GLFW_KEY_UNKNOWN) { return false; }
        return glfwGetKey(m_handle, glfwKey) == GLFW_PRESS;
    }

    void Window::requestClose() noexcept {
        glfwSetWindowShouldClose(m_handle,GLFW_TRUE);
    }
}
