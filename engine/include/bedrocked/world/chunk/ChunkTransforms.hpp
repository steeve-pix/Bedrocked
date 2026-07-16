#pragma once
#include "bedrocked/world/chunk/ChunkPosition.hpp"
#include "bedrocked/world/chunk/Chunk.hpp"
#include "bedrocked/math/Matrix4.hpp"

namespace bedrocked {
    [[nodiscard]] inline Matrix4 chunkModelMatrix(ChunkPosition position) noexcept {
        return Matrix4::translation(
            static_cast<float>(position.x) *
            static_cast<float>(Chunk::Width),

            static_cast<float>(position.y) *
            static_cast<float>(Chunk::Height),

            static_cast<float>(position.z) *
            static_cast<float>(Chunk::Depth)
        );
    }
} // namespace bedrocked