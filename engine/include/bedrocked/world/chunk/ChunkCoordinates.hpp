#pragma once
#include "Chunk.hpp"
#include "ChunkPosition.hpp"
#include "bedrocked/world/block/BlockPosition.hpp"

namespace bedrocked {
    [[nodiscard]] constexpr int floorDivide(int value, int divisor) noexcept {
        int quotient = value / divisor;
        const int remainder = value % divisor;

        if (remainder != 0 && ((remainder < 0) != (divisor < 0))) {
            --quotient;
        }
        return quotient;
    }

    [[nodiscard]] constexpr ChunkPosition chunkPositionForBlock(BlockPosition block) noexcept {
        return {
            .x = floorDivide(block.x, static_cast<int>(Chunk::Width)),
            .y = floorDivide(block.y, static_cast<int>(Chunk::Height)),
            .z = floorDivide(block.z, static_cast<int>(Chunk::Depth))
        };
    }
} //namespace bedrocked
