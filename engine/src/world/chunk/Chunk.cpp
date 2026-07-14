#include "../../../include/bedrocked/world/chunk/Chunk.hpp"

#include <cassert>

namespace bedrocked {
    BlockType Chunk::block(std::size_t x, std::size_t y, std::size_t z) const noexcept {
        return m_blocks[index(x, y, z)];
    }

    void Chunk::setBlock(int x, int y, int z, BlockType type) noexcept {
        assert(contains(x,y,z));
        m_blocks[index(x, y, z)] = type;
    }
}
