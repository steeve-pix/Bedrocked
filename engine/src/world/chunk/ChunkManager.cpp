#include "bedrocked/world/chunk/ChunkManager.hpp"

#include <cassert>

namespace bedrocked {
    Chunk &ChunkManager::createChunk(ChunkPosition position) {
        assert(chunkAt(position)== nullptr);

        m_chunks.push_back({.position = position, .chunk = {}});

        return m_chunks.back().chunk;
    }

    Chunk *ChunkManager::chunkAt(ChunkPosition position) noexcept {
        for (Entry &entry: m_chunks) {
            if (entry.position == position) {
                return &entry.chunk;
            }
        }

        return nullptr;
    }

    const Chunk *ChunkManager::chunkAt(ChunkPosition position) const noexcept {
        for (const Entry &entry: m_chunks) {
            if (entry.position == position) {
                return &entry.chunk;
            }
        }

        return nullptr;
    }

    ChunkNeighbors ChunkManager::neighborsOf(
        ChunkPosition position) const noexcept {
        return ChunkNeighbors{
            .front = chunkAt({position.x, position.y, position.z + 1}),

            .back = chunkAt({position.x, position.y, position.z - 1}),

            .left = chunkAt({position.x - 1, position.y, position.z}),

            .right = chunkAt({position.x + 1, position.y, position.z}),

            .top = chunkAt({position.x, position.y + 1, position.z}),

            .bottom = chunkAt({position.x, position.y - 1, position.z})
        };
    }

    std::vector<ChunkPosition> ChunkManager::positions() const {
        std::vector<ChunkPosition> result;
        result.reserve(m_chunks.size());

        for (const Entry &entry: m_chunks) {
            result.push_back(entry.position);
        }

        return result;
    }
} // namespace bedrocked
