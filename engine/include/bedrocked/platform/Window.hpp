#pragma once
#include "../input/Key.hpp"
#include <string>

struct GLFWwindow;

namespace bedrocked {
    struct WindowConfig {
        int width = 1280;
        int height = 720;
        std::string title = "Bedrocked";
    };

    struct FrameBufferSize {
        int width;
        int height;
    };

    struct CursorPosition {
        double x, y;
    };

    class Window final {
    public:
        explicit Window(const WindowConfig &config);

        ~Window();

        // Prevent copying to avoid accidental resource duplication or double deletion
        Window(const Window &) = delete;

        Window &operator=(const Window &) = delete;

        [[nodiscard]] bool shouldClose() const noexcept;

        void pollEvents() noexcept;

        [[nodiscard]] bool isKeyDown(Key key) const noexcept;

        void requestClose() noexcept;

        void swapBuffers() noexcept;

        [[nodiscard]] FrameBufferSize framebufferSize() const noexcept;

        [[nodiscard]] CursorPosition cursorPosition() const noexcept;

        void setCursorCaptured(bool captured) noexcept;

    private:
        GLFWwindow *m_handle{nullptr};
    };
} // namespace bedrocked
