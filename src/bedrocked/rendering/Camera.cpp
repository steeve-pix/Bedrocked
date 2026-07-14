#include "bedrocked/rendering/Camera.hpp"

namespace bedrocked {
    void Camera::move(float x, float y, float z) noexcept {
        m_x += x;
        m_y += y;
        m_z += z;
    }

    Matrix4 Camera::viewMatrix() const noexcept {
        return Matrix4::translation(-m_x, -m_y, -m_z);
    }
}
