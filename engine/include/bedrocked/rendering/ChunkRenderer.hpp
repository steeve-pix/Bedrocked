#pragma once

#include "bedrocked/math/Matrix4.hpp"
#include "bedrocked/rendering/Mesh.hpp"
#include "bedrocked/world/block/BlockPosition.hpp"
#include "bedrocked/world/chunk/ChunkPosition.hpp"

#include <memory>
#include <vector>

namespace bedrocked {

    class ChunkManager;
    class Renderer;
    class ShaderProgram;

    class ChunkRenderer final {
    public:
        void build(ChunkManager& chunkManager);

        void rebuildChunk(
            ChunkManager& chunkManager,
            ChunkPosition position);

        void rebuildAroundBlock(
            ChunkManager& chunkManager,
            BlockPosition block);

        void draw(
            Renderer& renderer,
            ShaderProgram& shader) const;

    private:
        struct RenderChunk {
            ChunkPosition position;
            Matrix4 model;
            std::unique_ptr<Mesh> mesh;
        };

        [[nodiscard]] RenderChunk* find(
            ChunkPosition position) noexcept;

        std::vector<RenderChunk> m_chunks;
    };

} // namespace bedrocked