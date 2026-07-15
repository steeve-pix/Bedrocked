#pragma once
#include <vector>

#include "Chunk.hpp"
#include "ChunkPosition.hpp"

namespace bedrocked {
    class ChunkManager final {
    public:
        Chunk &createChunk(ChunkPosition position);

        [[nodiscard]] Chunk *chunkAt(ChunkPosition position) noexcept;

        [[nodiscard]] const Chunk *chunkAt(ChunkPosition position) const noexcept;
    private:
        struct Entry {
            ChunkPosition position;
            Chunk chunk;
        };

        std::vector<Entry> m_chunks;
    };
}
