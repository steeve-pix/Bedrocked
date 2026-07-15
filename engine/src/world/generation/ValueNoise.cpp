#include "bedrocked/world/generation/ValueNoise.hpp"

#include <cmath>

namespace {
    [[nodiscard]] constexpr float smoothStep(float value) noexcept {
        return value * value * (3.0f - 2.0f * value);
    }

    [[nodiscard]] constexpr float lerp(float start, float end, float amount) noexcept {
        return start + (end - start) * amount;
    }
} // anonymous helper

namespace bedrocked {
    ValueNoise::ValueNoise(std::uint32_t seed) noexcept
        : m_seed(seed) {
    }

    float ValueNoise::valueAt(int x, int z) const noexcept {
        std::uint32_t hash = m_seed;

        hash ^= static_cast<std::uint32_t>(x) * 0x27D4EB2DU;
        hash ^= static_cast<std::uint32_t>(x) * 0x165667B1U;

        hash ^= hash >> 15U;
        hash *= 0x85EBCA6BU;
        hash ^= hash >> 13U;
        hash *= 0xC2B2AE35U;
        hash ^= hash >> 16U;

        constexpr float maximum24BitValue =
                static_cast<float>(0x00FFFFFFU);

        return static_cast<float>(hash >> 8U) / maximum24BitValue;
    }

    float ValueNoise::sample(float x, float z) const noexcept {
        const int minimumX =
                static_cast<int>(std::floor(x));

        const int minimumZ =
                static_cast<int>(std::floor(z));

        const int maximumX = minimumX + 1;
        const int maximumZ = minimumZ + 1;

        const float localX =
                x - static_cast<float>(minimumX);
        const float localZ =
                z - static_cast<float>(minimumZ);

        const float smoothX = smoothStep(localX);
        const float smoothZ = smoothStep(localZ);

        const float bottomLeft =
                valueAt(minimumX, minimumZ);

        const float bottomRight =
                valueAt(maximumX, maximumZ);

        const float topLeft =
                valueAt(minimumX, maximumZ);

        const float topRight =
                valueAt(maximumX, maximumZ);

        const float bottom =
                lerp(bottomLeft, bottomRight, smoothX);

        const float top =
                lerp(topLeft, topRight, smoothX);

        return lerp(bottom, top, smoothZ);
    }
} // namespace bedrocked
