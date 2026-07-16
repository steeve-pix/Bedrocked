#pragma once
#include "Mesh.hpp"

namespace bedrocked {
    class Renderer final {
    public:
        Renderer();

        void setClearColor(float red, float green, float blue, float alpha) noexcept;

        void clear() noexcept;

        void draw(const Mesh &mesh) noexcept;

        void setViewPort(int width, int height) noexcept;

        void drawLines(const Mesh &mesh) noexcept;
    };
} // namespace bedrocked
