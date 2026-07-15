#pragma once
#include "../math/Matrix4.hpp"

namespace bedrocked {
    class Camera final {
    public:
        void move(float x, float y, float z) noexcept;

        [[nodiscard]] Matrix4 viewMatrix() const noexcept;

        void rotate(float yawDelta, float pitchDelta) noexcept;

        void moveForward(float distance) noexcept;

        void moveRight(float distance) noexcept;

        void setPosition(float x, float y, float z) noexcept;

    private:
        float m_x{};
        float m_y{};
        float m_z{};

        float m_yaw{};
        float m_pitch{};
    };
} // namespace bedrocked
