#pragma once

#include <cstdint>

namespace bedrocked {
    class ValueNoise3D final {
    public:
        explicit ValueNoise3D(std::uint32_t seed) noexcept;

        [[nodiscard]] float sample(float x, float y, float z) const noexcept;

    private:
        [[nodiscard]] float valueAt(int x, int y, int z) const noexcept;

        std::uint32_t m_seed;
    };
} // namespace bedrocked
