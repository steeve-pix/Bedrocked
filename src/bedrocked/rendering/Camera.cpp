#include "bedrocked/rendering/Camera.hpp"

#include <algorithm>

namespace bedrocked {
    void Camera::move(float x, float y, float z) noexcept {
        m_x += x;
        m_y += y;
        m_z += z;
    }

    Matrix4 Camera::viewMatrix() const noexcept {
        const Matrix4 inverseTranslation = Matrix4::translation(-m_x, -m_y, -m_z);
        const Matrix4 inverseYaw = Matrix4::rotationY(-m_yaw);
        const Matrix4 inversePitch = Matrix4::rotationX(-m_pitch);

        return inversePitch * inverseYaw * inverseTranslation;
    }

    void Camera::rotate(float yawDelta, float pitchDelta) noexcept {
        m_yaw += yawDelta;
        m_pitch += pitchDelta;

        constexpr float maximumPitch = 1.553343F; // approximately 89°

        m_pitch = std::clamp(
            m_pitch,
            -maximumPitch,
            maximumPitch
        );
    }

    void Camera::moveForward(float distance) noexcept {
        m_x += std::sin(m_yaw) * distance;
        m_z -= std::cos(m_yaw) * distance;
    }

    void Camera::moveRight(float distance) noexcept {
        m_x += std::cos(m_yaw) * distance;
        m_z += std::sin(m_yaw) * distance;
    }
} // namespace bedrocked
