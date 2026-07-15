#pragma once

#include "bedrocked/world/chunk/Chunk.hpp"
#include <algorithm>
#include <cmath>

#include "FractalNoise.hpp"


namespace bedrocked {
    [[nodiscard]] inline int terrainHeight(const FractalNoise &noise, int worldX, int worldZ) noexcept {
        constexpr float terrainScale = 0.04f;

        constexpr int octaveCount = 4;
        constexpr float persistence = 0.5f;
        constexpr float lacunarity = 2.0f;

        constexpr int minimumHeight = 2;
        constexpr int heightRange = 10;

        const float noiseValue = noise.sample(
            static_cast<float>(worldX) * terrainScale,
            static_cast<float>(worldZ) * terrainScale,
            octaveCount, persistence,
            lacunarity
        );

        const int height =
                minimumHeight + static_cast<int>(std::round(noiseValue * static_cast<float>(heightRange)));

        return std::clamp(height, 1, static_cast<int>(Chunk::Height) - 1);
    }
} // namespace bedrocked
