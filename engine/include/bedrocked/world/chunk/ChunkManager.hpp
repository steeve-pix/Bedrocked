#pragma once
#include <deque>
#include <vector>

#include "Chunk.hpp"
#include "ChunkNeighbors.hpp"
#include "ChunkPosition.hpp"

namespace bedrocked {
    class ChunkManager final {
    public:
        Chunk &createChunk(ChunkPosition position);

        [[nodiscard]] Chunk *chunkAt(ChunkPosition position) noexcept;

        [[nodiscard]] const Chunk *chunkAt(ChunkPosition position) const noexcept;

        [[nodiscard]] ChunkNeighbors neighborsOf(
    ChunkPosition position) const noexcept;
    private:
        struct Entry {
            ChunkPosition position;
            Chunk chunk;
        };

        std::deque<Entry> m_chunks;
    };
}
