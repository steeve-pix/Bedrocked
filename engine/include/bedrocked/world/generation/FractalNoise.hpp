#pragma once

#include "bedrocked/world/generation/ValueNoise.hpp"

#include <cstdint>

namespace bedrocked {
    class FractalNoise final {
    public:
        explicit FractalNoise(std::uint32_t seed) noexcept;

        [[nodiscard]] float sample(float x, float z, int octaveCount, float persistence,
                                   float lacunarity) const noexcept;

    private:
        ValueNoise m_valueNoise;
    };
} // namespace bedrocked
