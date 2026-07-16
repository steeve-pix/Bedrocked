#include "bedrocked/rendering/ChunkRenderer.hpp"

#include "bedrocked/world/chunk/ChunkManager.hpp"
#include "bedrocked/world/chunk/ChunkMesher.hpp"
#include "bedrocked/world/chunk/ChunkTransforms.hpp"
#include "bedrocked/rendering/Renderer.hpp"
#include "bedrocked/rendering/opengl/ShaderProgram.hpp"
#include "bedrocked/world/chunk/ChunkCoordinates.hpp"

#include <array>

namespace bedrocked {
    ChunkRenderer::RenderChunk *ChunkRenderer::find(
        ChunkPosition position) noexcept {
        for (RenderChunk &renderChunk: m_chunks) {
            if (renderChunk.position == position) {
                return &renderChunk;
            }
        }

        return nullptr;
    }

    void ChunkRenderer::build(ChunkManager &chunkManager) {
        m_chunks.clear();

        const std::vector<ChunkPosition> positions =
                chunkManager.positions();

        m_chunks.reserve(positions.size());

        for (const ChunkPosition position: positions) {
            Chunk *chunk =
                    chunkManager.chunkAt(position);

            if (chunk == nullptr) {
                continue;
            }

            const ChunkNeighbors neighbors =
                    chunkManager.neighborsOf(position);

            const ChunkMeshData meshData =
                    buildChunkMeshData(
                        *chunk,
                        neighbors
                    );

            std::unique_ptr<Mesh> mesh;

            if (!meshData.empty()) {
                mesh = std::make_unique<Mesh>(
                    meshData.vertices.data(),
                    meshData.vertices.size(),
                    meshData.indices.data(),
                    meshData.indices.size()
                );
            }

            m_chunks.push_back({
                .position = position,
                .model = chunkModelMatrix(position),
                .mesh = std::move(mesh)
            });
        }
    }

    void ChunkRenderer::rebuildChunk(ChunkManager &chunkManager, ChunkPosition position) {
        Chunk *chunk =
                chunkManager.chunkAt(position);

        if (chunk == nullptr) {
            return;
        }

        const ChunkNeighbors neighbors =
                chunkManager.neighborsOf(position);

        const ChunkMeshData meshData =
                buildChunkMeshData(
                    *chunk,
                    neighbors
                );

        RenderChunk *renderChunk =
                find(position);

        if (renderChunk == nullptr) {
            std::unique_ptr<Mesh> mesh;

            if (!meshData.empty()) {
                mesh = std::make_unique<Mesh>(
                    meshData.vertices.data(),
                    meshData.vertices.size(),
                    meshData.indices.data(),
                    meshData.indices.size()
                );
            }

            m_chunks.push_back({
                .position = position,
                .model = chunkModelMatrix(position),
                .mesh = std::move(mesh)
            });

            return;
        }

        if (meshData.empty()) {
            renderChunk->mesh.reset();
            return;
        }

        renderChunk->mesh =
                std::make_unique<Mesh>(
                    meshData.vertices.data(),
                    meshData.vertices.size(),
                    meshData.indices.data(),
                    meshData.indices.size()
                );
    }

    void ChunkRenderer::rebuildAroundBlock(ChunkManager &chunkManager, BlockPosition block) {
        const ChunkPosition owningChunk =
                chunkPositionForBlock(block);

        constexpr std::array offsets{
            ChunkPosition{0, 0, 0},
            ChunkPosition{1, 0, 0},
            ChunkPosition{-1, 0, 0},
            ChunkPosition{0, 1, 0},
            ChunkPosition{0, -1, 0},
            ChunkPosition{0, 0, 1},
            ChunkPosition{0, 0, -1}
        };

        for (const ChunkPosition offset: offsets) {
            rebuildChunk(
                chunkManager,
                ChunkPosition{
                    .x = owningChunk.x + offset.x,
                    .y = owningChunk.y + offset.y,
                    .z = owningChunk.z + offset.z
                }
            );
        }
    }

    void ChunkRenderer::draw(Renderer &renderer, ShaderProgram &shader) const {
        for (const RenderChunk &renderChunk: m_chunks) {
            if (renderChunk.mesh == nullptr) {
                continue;
            }

            shader.setMat4(
                "model",
                renderChunk.model.data()
            );

            renderer.draw(
                *renderChunk.mesh
            );
        }
    }
}
