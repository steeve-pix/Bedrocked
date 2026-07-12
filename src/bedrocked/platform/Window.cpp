#include "Window.hpp"
#include <iostream>

#include <GLFW/glfw3.h>

namespace bedrocked {
    Window::Window(const WindowConfig &config) {
        // Initialize GLFW
        if (!glfwInit()) {
            throw std::runtime_error{"Failed to initialize GLFW"};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create the window
        m_handle = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);

        if (!m_handle) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        // Bind the OpenGL context to current threaad
        glfwMakeContextCurrent(m_handle);
    }

    Window::~Window() {
        if (m_handle)
            glfwDestroyWindow(m_handle);
        glfwTerminate();
    }

    bool Window::ShouldClose() const noexcept {
        return glfwWindowShouldClose(m_handle);
    }

    void Window::pollEvents() noexcept {
        glfwPollEvents();
    }
}
