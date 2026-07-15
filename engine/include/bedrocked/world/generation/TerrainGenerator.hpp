#pragma once

#include "bedrocked/world/chunk/Chunk.hpp"
#include "bedrocked/world/chunk/ChunkPosition.hpp"
#include "bedrocked/world/generation/FractalNoise.hpp"

#include <cstdint>

namespace bedrocked {
    class TerrainGenerator final {
    public:
        explicit TerrainGenerator(std::uint32_t seed) noexcept;

        void generate(Chunk &chunk, ChunkPosition position) const;

        [[nodiscard]] int surfaceHeightAt(int worldX,int worldZ) const noexcept;

    private:
        FractalNoise m_noise;
    };
} // namespace bedrocked
