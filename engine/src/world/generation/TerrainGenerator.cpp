#include "bedrocked/world/generation/TerrainGenerator.hpp"

#include "../../../../cmake-build-debug/_deps/glfw-src/src/internal.h"
#include "bedrocked/world/generation/TerrainHeight.hpp"

namespace bedrocked {
    TerrainGenerator::TerrainGenerator(std::uint32_t seed) noexcept
        : m_noise(seed) {
    }

    void TerrainGenerator::generate(Chunk &chunk, ChunkPosition position) const {
        for (int localZ{}; localZ < static_cast<int>(Chunk::Depth); ++localZ) {
            for (int localX{}; localX < static_cast<int>(Chunk::Width); ++localX) {
                const int worldX =
                        position.x * static_cast<int>(Chunk::Width) + localX;
                const int worldZ =
                        position.z * static_cast<int>(Chunk::Depth) + localZ;

                const int surfaceHeight =
                        surfaceHeightAt(worldX, worldZ);

                for (int localY{}; localY <= surfaceHeight; ++localY) {
                    BlockType blockType = BlockType::Stone;

                    if (localY == surfaceHeight) {
                        blockType = BlockType::Grass;
                    } else if (localY >= surfaceHeight - 3) {
                        blockType = BlockType::Dirt;
                    }

                    chunk.setBlock(localX, localY, localZ, blockType);
                }
            }
        }
    }

    int TerrainGenerator::surfaceHeightAt(int worldX, int worldZ) const noexcept {
        return terrainHeight(m_noise, worldX, worldZ);
    }
} // namespace bedrocked
