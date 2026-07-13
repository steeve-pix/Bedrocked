#pragma once
#include <array>

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

        [[nodiscard]] constexpr const float *data() const noexcept {
            return m_values.data();
        }

    private:
        explicit constexpr Matrix4(std::array<float, 16> values) noexcept
            : m_values(values) {
        }

        std::array<float, 16> m_values{};
    };
}
