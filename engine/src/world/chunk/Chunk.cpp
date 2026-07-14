#include "../../../include/bedrocked/world/chunk/Chunk.hpp"

namespace bedrocked {
    BlockType Chunk::block(std::size_t x, std::size_t y, std::size_t z) const noexcept {
        return m_blocks[index(x, y, z)];
    }

    void Chunk::setBlock(std::size_t x, std::size_t y, std::size_t z, BlockType type) noexcept {
        m_blocks[index(x, y, z)] = type;
    }
}
