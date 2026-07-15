#include "bedrocked/world/generation/TerrainGenerator.hpp"

#include "bedrocked/world/generation/TerrainHeight.hpp"

namespace bedrocked {
    TerrainGenerator::TerrainGenerator(std::uint32_t seed) noexcept
        : m_noise(seed), m_caveNoise(seed ^ 0x9E3779B9U) {
    }

    void TerrainGenerator::generate(Chunk &chunk, ChunkPosition position) const {
        constexpr float caveScale = 0.18F;
        constexpr float caveThreshold = 0.52F;
        constexpr int surfaceProtectionDepth = 2;

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

                    const int worldY =
                            position.y * static_cast<int>(Chunk::Height) + localY;

                    const bool canCarveCave =
                            blockType == BlockType::Stone && localY > 0 && localY < surfaceHeight -
                            surfaceProtectionDepth;

                    if (canCarveCave) {
                        const float caveValue =
                                m_caveNoise.sample(
                                    static_cast<float>(worldX) * caveScale,
                                    static_cast<float>(worldY) * caveScale,
                                    static_cast<float>(worldZ) * caveScale
                                );

                        if (caveValue > caveThreshold) {
                            continue;
                        }
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
