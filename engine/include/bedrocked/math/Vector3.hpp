#pragma once

namespace bedrocked {
    struct Vector3 {
        double x{},
                y{},
                z{};

        constexpr Vector3 &operator+=(const Vector3 &other) noexcept {
            x += other.x,
                    y += other.y,
                    z += other.z;
            return *this;
        }
    };

    [[nodiscard]] constexpr Vector3 operator*(Vector3 vector, float scalar) noexcept {
        return {
            vector.x * scalar,
            vector.y * scalar,
            vector.z * scalar
        };
    }
} // namespace bedrocked
