#pragma once

struct GLFWwindow;

namespace bedrocked {
    class ImGuiLayer final {
    public:
        explicit ImGuiLayer(GLFWwindow *window);

        ~ImGuiLayer();

        ImGuiLayer(const ImGuiLayer &) = delete;

        ImGuiLayer &operator=(const ImGuiLayer &) = delete;

        void beginFrame() noexcept;

        void endFrame() noexcept;
    };
} // namespace bedrocked
