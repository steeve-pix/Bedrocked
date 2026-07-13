#pragma once
#include "Mesh.hpp"

namespace bedrocked {
    class Renderer final {
    public:
        void setClearColor(float red, float green, float blue, float alpha) noexcept;

        void clear() noexcept;

        void draw(const Mesh &mesh) noexcept;
    };
}
