#pragma once
#include <array>
#include <cmath>
#include <cstddef>

namespace bedrocked {
    class Matrix4 final {
    public:
        [[nodiscard]] static constexpr Matrix4 identity() noexcept {
            return Matrix4{
                {
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                }
            };
        }

        [[nodiscard]] static constexpr Matrix4 translation(float x, float y, float z) noexcept {
            return Matrix4{
                {
                    1.0F, 0.0F, 0.0F, 0.0F,
                    0.0F, 1.0F, 0.0F, 0.0F,
                    0.0F, 0.0F, 1.0F, 0.0F,
                    x, y, z, 1.0F
                }
            };
        }

        [[nodiscard]] static Matrix4 rotationX(float angleRadians) noexcept {
            const float cosine = std::cos(angleRadians);
            const float sine = std::sin(angleRadians);

            return Matrix4{
                {
                    1.0F, 0.0F, 0.0F, 0.0F,
                    0.0F, cosine, sine, 0.0F,
                    0.0F, -sine, cosine, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F
                }
            };
        }

        [[nodiscard]] static Matrix4 rotationY(float angleRadians) noexcept {
            const float cosine = std::cos(angleRadians);
            const float sine = std::sin(angleRadians);

            return Matrix4{
                {
                    cosine, 0.0F, -sine, 0.0F,
                    0.0F, 1.0F, 0.0F, 0.0F,
                    sine, 0.0F, cosine, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F
                }
            };
        }

        [[nodiscard]] static Matrix4 rotationZ(float angleRadians) noexcept {
            const float cosine = std::cos(angleRadians);
            const float sine = std::sin(angleRadians);

            return Matrix4{
                {
                    cosine, sine, 0.0F, 0.0F,
                    -sine, cosine, 0.0F, 0.0F,
                    0.0F, 0.0F, 1.0F, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F
                }
            };
        }

        [[nodiscard]] constexpr const float *data() const noexcept {
            return m_values.data();
        }

        [[nodiscard]] constexpr Matrix4 operator*(const Matrix4 &other) const noexcept {
            std::array < float, 16 > result{};
            for (std::size_t column = 0; column < 4; ++column) {
                for (std::size_t row = 0; row < 4; ++row) {
                    for (std::size_t index = 0; index < 4; ++index) {
                        result[column * 4 + row] +=
                                m_values[index * 4 + row] *
                                other.m_values[column * 4 + index];
                    }
                }
            }
            return Matrix4{result};
        }

        [[nodiscard]] static constexpr Matrix4 scale(float x, float y, float z) noexcept {
            return Matrix4{
                {
                    x, 0.0F, 0.0F, 0.0F,
                    0.0F, y, 0.0F, 0.0F,
                    0.0F, 0.0F, z, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F
                }
            };
        }

        [[nodiscard]] static constexpr Matrix4 aspectCorrection(float aspectRatio) noexcept {
            return Matrix4{
                {
                    1.0F / aspectRatio, 0.0F, 0.0F, 0.0F,
                    0.0F, 1.0F, 0.0F, 0.0F,
                    0.0F, 0.0F, 1.0F, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F
                }
            };
        }

        [[nodiscard]] static Matrix4 perspective(
            float verticalFieldOfViewRadians,
            float aspectRatio,
            float nearPlane,
            float farPlane) noexcept {
            const float focalLength =
                    1.0F / std::tan(verticalFieldOfViewRadians / 2.0F);

            return Matrix4{
                {
                    focalLength / aspectRatio, 0.0F, 0.0F, 0.0F,
                    0.0F, focalLength, 0.0F, 0.0F,
                    0.0F, 0.0F, (farPlane + nearPlane) / (nearPlane - farPlane), -1.0F,
                    0.0F, 0.0F, (2.0F * farPlane * nearPlane) / (nearPlane - farPlane), 0.0F
                }
            };
        }

    private:
        explicit constexpr Matrix4(std::array<float, 16> values) noexcept
            : m_values(values) {
        }

        std::array<float, 16> m_values{};
    };
} // namespace bedrocked
