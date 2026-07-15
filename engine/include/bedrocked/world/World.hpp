#pragma once
#include "chunk/ChunkManager.hpp"
#include "generation/TerrainGenerator.hpp"

namespace bedrocked {
    class World final {
    public:
        explicit World(std::uint32_t seed) noexcept;

        void generateTestWorld();

        void setBlockAtWorld(int worldX, int worldY, int worldZ, BlockType type) noexcept;

        [[nodiscard]] ChunkManager &chunks() noexcept;

        [[nodiscard]] const ChunkManager &chunks() const noexcept;

    private:
        std::uint32_t m_seed;
        ChunkManager m_chunkManager;
        TerrainGenerator m_terrainGenerator;
    };
} // namespace bedrocked
