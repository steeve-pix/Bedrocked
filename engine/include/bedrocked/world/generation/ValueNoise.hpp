#pragma once

#include <cstdint>

namespace bedrocked {
    class ValueNoise final {
    public:
        explicit ValueNoise(std::uint32_t seed) noexcept;

        [[nodiscard]] float sample(float x, float z) const noexcept;

    private:
        [[nodiscard]] float valueAt(int x, int z) const noexcept;

        std::uint32_t m_seed;
    };
}// namespace bedrocked
