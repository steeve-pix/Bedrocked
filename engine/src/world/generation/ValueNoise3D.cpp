#include "bedrocked/world/generation/ValueNoise3D.hpp"

#include <cmath>

namespace {
    [[nodiscard]] constexpr float fade(float value) noexcept {
        return value * value * value *
               (value * (value * 6.0F - 15.0F) + 10.0F);
    }

    [[nodiscard]] constexpr float lerp(
        float start,
        float end,
        float amount) noexcept {
        return start + (end - start) * amount;
    }
} // namespace
namespace bedrocked {
    ValueNoise3D::ValueNoise3D(std::uint32_t seed) noexcept
        : m_seed(seed) {
    }

    float ValueNoise3D::valueAt(int x, int y, int z) const noexcept {
        std::uint32_t hash = m_seed;

        hash ^= static_cast<std::uint32_t>(x) * 0x27D4EB2DU;
        hash ^= static_cast<std::uint32_t>(y) * 0x9E3779B9U;
        hash ^= static_cast<std::uint32_t>(z) * 0x165667B1U;

        hash ^= hash >> 15U;
        hash *= 0x85EBCA6BU;
        hash ^= hash >> 13U;
        hash *= 0xC2B2AE35U;
        hash ^= hash >> 16U;

        constexpr float maximum24BitValue =
                static_cast<float>(0x00FFFFFFU);

        return static_cast<float>(hash >> 8U) / maximum24BitValue;
    }

    float ValueNoise3D::sample(
        float x,
        float y,
        float z) const noexcept {
        const int minimumX =
                static_cast<int>(std::floor(x));

        const int minimumY =
                static_cast<int>(std::floor(y));

        const int minimumZ =
                static_cast<int>(std::floor(z));

        const int maximumX = minimumX + 1;
        const int maximumY = minimumY + 1;
        const int maximumZ = minimumZ + 1;

        const float localX =
                x - static_cast<float>(minimumX);

        const float localY =
                y - static_cast<float>(minimumY);

        const float localZ =
                z - static_cast<float>(minimumZ);

        const float smoothX = fade(localX);
        const float smoothY = fade(localY);
        const float smoothZ = fade(localZ);

        const float bottomBackLeft =
                valueAt(minimumX, minimumY, minimumZ);

        const float bottomBackRight =
                valueAt(maximumX, minimumY, minimumZ);

        const float topBackLeft =
                valueAt(minimumX, maximumY, minimumZ);

        const float topBackRight =
                valueAt(maximumX, maximumY, minimumZ);

        const float bottomFrontLeft =
                valueAt(minimumX, minimumY, maximumZ);

        const float bottomFrontRight =
                valueAt(maximumX, minimumY, maximumZ);

        const float topFrontLeft =
                valueAt(minimumX, maximumY, maximumZ);

        const float topFrontRight =
                valueAt(maximumX, maximumY, maximumZ);

        const float bottomBack =
                lerp(bottomBackLeft, bottomBackRight, smoothX);

        const float topBack =
                lerp(topBackLeft, topBackRight, smoothX);

        const float bottomFront =
                lerp(bottomFrontLeft, bottomFrontRight, smoothX);

        const float topFront =
                lerp(topFrontLeft, topFrontRight, smoothX);

        const float back =
                lerp(bottomBack, topBack, smoothY);

        const float front =
                lerp(bottomFront, topFront, smoothY);

        return lerp(
            back, front, smoothZ);
    }
} // namespace bedrocked
