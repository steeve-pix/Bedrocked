#include "../../include/bedrocked/rendering/Camera.hpp"

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

    void Camera::setPosition(float x, float y, float z) noexcept {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    float Camera::yaw() const noexcept {
        return m_yaw;
    }

    float Camera::pitch() const noexcept {
        return m_pitch;
    }

    Vector3 Camera::forwardDirection() const noexcept {
        const float horizontalLength =
                std::cos(m_pitch);

        return Vector3{
            .x = -horizontalLength * std::sin(m_yaw),
            .y = std::sin(m_pitch),
            .z = -horizontalLength * std::cos(m_yaw)
        };
    }
} // namespace bedrocked
