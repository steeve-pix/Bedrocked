#include "../../../include/bedrocked/world/chunk/Chunk.hpp"

#include <cassert>

namespace bedrocked {
    BlockType Chunk::block(int x, int y, int z) const noexcept {
        return m_blocks[index(x, y, z)];
    }

    BlockType Chunk::blockOrAir(int x, int y, int z) const noexcept {
        if (!contains(x, y, z)) {
            return BlockType::Air;
        }

        return block(x, y, z);
    }

    void Chunk::setBlock(int x, int y, int z, BlockType type) noexcept {
        assert(contains(x, y, z));
        m_blocks[index(x, y, z)] = type;
    }
}
