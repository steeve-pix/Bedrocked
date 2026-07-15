#pragma once

#include <algorithm>
#include <cmath>

#include "bedrocked/world/chunk/Chunk.hpp"

namespace bedrocked {
    [[nodiscard]] inline int terrainHeight(int worldX, int worldZ) noexcept {
        constexpr float baseHeight = 5.0f;
        constexpr float amplitude = 3.0f;
        constexpr float frequency = 0.12f;

        const float xWave =
                std::sin(static_cast<float>(worldX) * frequency);

        const float zWave =
                std::cos(static_cast<float>(worldZ) * frequency);

        const float height =
                baseHeight * amplitude * 0.5f * (xWave * zWave);

        return std::clamp(static_cast<int>(std::round(height)), 1, static_cast<int>(Chunk::Height) - 1);
    }
} // namespace bedrocked
