#pragma once
#include "bedrocked/math/Matrix4.hpp"

namespace bedrocked {
    class Camera final {
    public:
        void move(float x, float y, float z) noexcept;

        [[nodiscard]] Matrix4 viewMatrix() const noexcept;

    private:
        float m_x{};
        float m_y{};
        float m_z{};
    };
}
