#include "../../include/bedrocked/platform/Window.hpp"

#include <glad/glad.h>
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

            case bedrocked::Key::Digit1: return GLFW_KEY_1;
            case bedrocked::Key::Digit2: return GLFW_KEY_2;
            case bedrocked::Key::Digit3: return GLFW_KEY_3;
            case bedrocked::Key::Digit4: return GLFW_KEY_4;
            case bedrocked::Key::Digit5: return GLFW_KEY_5;
        }
        return GLFW_KEY_UNKNOWN;
    }

    [[nodiscard]] int toGlfwMouseButton(bedrocked::MouseButton button) noexcept {
        switch (button) {
            case bedrocked::MouseButton::Left:
                return GLFW_MOUSE_BUTTON_LEFT;

            case bedrocked::MouseButton::Right:
                return GLFW_MOUSE_BUTTON_RIGHT;
        }

        return GLFW_MOUSE_BUTTON_LEFT;
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
        m_handle = glfwCreateWindow(
            config.width,
            config.height,
            config.title.c_str(),
            nullptr,
            nullptr
        );

        if (!m_handle) {
            glfwTerminate();
            throw std::runtime_error{"Failed to create GLFW window"};
        }

        // Bind the OpenGL context to current threaad
        glfwMakeContextCurrent(m_handle);

        // Load OpenGL
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
            glfwDestroyWindow(m_handle);
            m_handle = nullptr;
            glfwTerminate();

            throw std::runtime_error{"Failed to initialize GLAD"};
        }
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

    void Window::swapBuffers() noexcept {
        glfwSwapBuffers(m_handle);
    }

    FrameBufferSize Window::framebufferSize() const noexcept {
        FrameBufferSize size{};

        glfwGetFramebufferSize(m_handle, &size.width, &size.height);

        return size;
    }

    CursorPosition Window::cursorPosition() const noexcept {
        CursorPosition position{};

        glfwGetCursorPos(m_handle, &position.x, &position.y);
        return position;
    }

    void Window::setCursorCaptured(bool captured) noexcept {
        glfwSetInputMode(m_handle,GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    bool Window::isMouseButtonDown(MouseButton button) const noexcept {
        return glfwGetMouseButton(m_handle, toGlfwMouseButton(button)) == GLFW_PRESS;
    }

    GLFWwindow *Window::nativeHandle() const noexcept {
        return m_handle;
    }
} // namespace bedrocked
