#include "bedrocked/world/generation/FractalNoise.hpp"

#include <cmath>

namespace bedrocked {
    FractalNoise::FractalNoise(std::uint32_t seed) noexcept
        : m_valueNoise(seed) {
    }

    float FractalNoise::sample(float x, float z, int octaveCount, float persistence, float lacunarity) const noexcept {
        if (octaveCount <= 0) {
            return 0.0f;
        }

        float total{};
        float totalAmplitude{};
        float frequency{1.0f};
        float amplitude{1.0f};

        for (int octave{}; octave < octaveCount; ++octave) {
            const float angle =
                    static_cast<float>(octave) * 0.73f;

            const float cosine = std::cos(angle);
            const float sine = std::sin(angle);

            const float rotatedX =
                    x * cosine - z * sine;

            const float rotatedZ =
                    x * sine + z * cosine;

            const float offset =
                    static_cast<float>(octave) * 19.37f;

            const float noiseValue =
                    m_valueNoise.sample(rotatedX * frequency + offset, rotatedZ * frequency - offset);

            total += noiseValue * amplitude;
            totalAmplitude += amplitude;

            frequency *= lacunarity;
            amplitude *= lacunarity;
        }

        return totalAmplitude > 0.0f ? total / totalAmplitude : 0.0f;
    }
} // namespace bedrocked
