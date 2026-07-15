#include "bedrocked/world/chunk/ChunkMesher.hpp"

#include "bedrocked/rendering/texture/TextureAtlasLayout.hpp"
#include "bedrocked/rendering/texture/TextureRegion.hpp"
#include "bedrocked/world/block/BlockProperties.hpp"
#include "bedrocked/world/chunk/BlockFace.hpp"
#include "bedrocked/world/chunk/ChunkMeshing.hpp"

#include <array>
#include <cstdint>

namespace {
    constexpr bedrocked::TextureAtlasLayout kAtlasLayout{2, 2};

    constexpr bedrocked::TextureRegion kDirtRegion =
            kAtlasLayout.region(0, 0);

    constexpr bedrocked::TextureRegion kWoodRegion =
            kAtlasLayout.region(1, 0);

    constexpr bedrocked::TextureRegion kGrassRegion =
            kAtlasLayout.region(0, 1);

    constexpr bedrocked::TextureRegion kStoneRegion =
            kAtlasLayout.region(1, 1);

    [[nodiscard]] constexpr bedrocked::TextureRegion textureRegionFor(
        bedrocked::BlockType type,
        bedrocked::BlockFace face) noexcept {
        switch (type) {
            case bedrocked::BlockType::Grass:
                if (face == bedrocked::BlockFace::Top) {
                    return kGrassRegion;
                }

                return kDirtRegion;

            case bedrocked::BlockType::Dirt:
                return kDirtRegion;

            case bedrocked::BlockType::Stone:
                return kStoneRegion;

            case bedrocked::BlockType::Wood:
                return kWoodRegion;

            case bedrocked::BlockType::Air:
                return kDirtRegion;
        }

        return kDirtRegion;
    }

    void appendFace(
        bedrocked::ChunkMeshData &meshData,
        int blockX,
        int blockY,
        int blockZ,
        bedrocked::BlockFace face,
        const bedrocked::TextureRegion &textureRegion) {
        const float minimumX = static_cast<float>(blockX);
        const float minimumY = static_cast<float>(blockY);
        const float minimumZ = static_cast<float>(blockZ);

        const float maximumX = minimumX + 1.0F;
        const float maximumY = minimumY + 1.0F;
        const float maximumZ = minimumZ + 1.0F;

        const auto baseIndex =
                static_cast<std::uint32_t>(meshData.vertices.size());

        const auto addVertex =
                [&meshData](
            float x,
            float y,
            float z,
            float u,
            float v) {
            meshData.vertices.push_back({
                .position = {x, y, z},
                .color = {1.0F, 1.0F, 1.0F},
                .textureCoordinates = {u, v}
            });
        };

        switch (face) {
            case bedrocked::BlockFace::Front:
                addVertex(
                    minimumX, minimumY, maximumZ,
                    textureRegion.minimumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, minimumY, maximumZ,
                    textureRegion.maximumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, maximumY, maximumZ,
                    textureRegion.maximumU,
                    textureRegion.maximumV
                );

                addVertex(
                    minimumX, maximumY, maximumZ,
                    textureRegion.minimumU,
                    textureRegion.maximumV
                );
                break;

            case bedrocked::BlockFace::Back:
                addVertex(
                    maximumX, minimumY, minimumZ,
                    textureRegion.minimumU,
                    textureRegion.minimumV
                );

                addVertex(
                    minimumX, minimumY, minimumZ,
                    textureRegion.maximumU,
                    textureRegion.minimumV
                );

                addVertex(
                    minimumX, maximumY, minimumZ,
                    textureRegion.maximumU,
                    textureRegion.maximumV
                );

                addVertex(
                    maximumX, maximumY, minimumZ,
                    textureRegion.minimumU,
                    textureRegion.maximumV
                );
                break;

            case bedrocked::BlockFace::Left:
                addVertex(
                    minimumX, minimumY, minimumZ,
                    textureRegion.minimumU,
                    textureRegion.minimumV
                );

                addVertex(
                    minimumX, minimumY, maximumZ,
                    textureRegion.maximumU,
                    textureRegion.minimumV
                );

                addVertex(
                    minimumX, maximumY, maximumZ,
                    textureRegion.maximumU,
                    textureRegion.maximumV
                );

                addVertex(
                    minimumX, maximumY, minimumZ,
                    textureRegion.minimumU,
                    textureRegion.maximumV
                );
                break;

            case bedrocked::BlockFace::Right:
                addVertex(
                    maximumX, minimumY, maximumZ,
                    textureRegion.minimumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, minimumY, minimumZ,
                    textureRegion.maximumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, maximumY, minimumZ,
                    textureRegion.maximumU,
                    textureRegion.maximumV
                );

                addVertex(
                    maximumX, maximumY, maximumZ,
                    textureRegion.minimumU,
                    textureRegion.maximumV
                );
                break;

            case bedrocked::BlockFace::Top:
                addVertex(
                    minimumX, maximumY, maximumZ,
                    textureRegion.minimumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, maximumY, maximumZ,
                    textureRegion.maximumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, maximumY, minimumZ,
                    textureRegion.maximumU,
                    textureRegion.maximumV
                );

                addVertex(
                    minimumX, maximumY, minimumZ,
                    textureRegion.minimumU,
                    textureRegion.maximumV
                );
                break;

            case bedrocked::BlockFace::Bottom:
                addVertex(
                    minimumX, minimumY, minimumZ,
                    textureRegion.minimumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, minimumY, minimumZ,
                    textureRegion.maximumU,
                    textureRegion.minimumV
                );

                addVertex(
                    maximumX, minimumY, maximumZ,
                    textureRegion.maximumU,
                    textureRegion.maximumV
                );

                addVertex(
                    minimumX, minimumY, maximumZ,
                    textureRegion.minimumU,
                    textureRegion.maximumV
                );
                break;
        }

        meshData.indices.insert(
            meshData.indices.end(),
            {
                baseIndex,
                baseIndex + 1,
                baseIndex + 2,

                baseIndex,
                baseIndex + 2,
                baseIndex + 3
            }
        );
    }
} // namespace

namespace bedrocked {
    ChunkMeshData buildChunkMeshData(const Chunk &chunk, const ChunkNeighbors &neighbors) {
        ChunkMeshData meshData;

        constexpr std::array faces{
            BlockFace::Front,
            BlockFace::Back,
            BlockFace::Left,
            BlockFace::Right,
            BlockFace::Top,
            BlockFace::Bottom
        };

        for (int y = 0; y < static_cast<int>(Chunk::Height); ++y) {
            for (int z = 0; z < static_cast<int>(Chunk::Depth); ++z) {
                for (int x = 0; x < static_cast<int>(Chunk::Width); ++x) {
                    const BlockType type = chunk.block(x, y, z);

                    if (isAir(type)) {
                        continue;
                    }

                    for (const BlockFace face: faces) {
                        if (!isFaceVisible(chunk, neighbors, x, y, z, face)) {
                            continue;
                        }

                        appendFace(
                            meshData,
                            x,
                            y,
                            z,
                            face,
                            textureRegionFor(type, face)
                        );
                    }
                }
            }
        }

        return meshData;
    }
} // namespace bedrocked
