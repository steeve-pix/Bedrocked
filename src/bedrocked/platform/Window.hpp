#pragma once
#include <string>
#include <stdexcept>

struct GLFWwindow;

namespace bedrocked {
    struct WindowConfig {
        int width = 1280;
        int height = 720;
        std::string title = "Bedrocked";
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

    private:
        GLFWwindow *m_handle{nullptr};
    };
}
