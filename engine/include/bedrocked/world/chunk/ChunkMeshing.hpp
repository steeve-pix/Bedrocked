#pragma once

#include "ChunkNeighbors.hpp"
#include "bedrocked/world/block/BlockProperties.hpp"
#include "bedrocked/world/chunk/BlockFace.hpp"
#include "bedrocked/world/chunk/Chunk.hpp"

namespace bedrocked {
    [[nodiscard]] inline BlockType blockFromNeighborhood(const Chunk &chunk, const ChunkNeighbors &neighbors, int x,
                                                         int y, int z) noexcept {
        if (Chunk::contains(x, y, z)) {
            return chunk.block(x, y, z);
        }

        if (x < 0) {
            if (neighbors.left == nullptr) {
                return BlockType::Air;
            }

            return neighbors.left->block(
                static_cast<int>(Chunk::Width) - 1,
                y,
                z
            );
        }

        if (x >= static_cast<int>(Chunk::Width)) {
            if (neighbors.right == nullptr) {
                return BlockType::Air;
            }

            return neighbors.right->block(0, y, z);
        }

        if (y < 0) {
            if (neighbors.bottom == nullptr) {
                return BlockType::Air;
            }

            return neighbors.bottom->block(
                x,
                static_cast<int>(Chunk::Height) - 1,
                z
            );
        }

        if (y >= static_cast<int>(Chunk::Height)) {
            if (neighbors.top == nullptr) {
                return BlockType::Air;
            }

            return neighbors.top->block(x, 0, z);
        }

        if (z < 0) {
            if (neighbors.back == nullptr) {
                return BlockType::Air;
            }

            return neighbors.back->block(
                x,
                y,
                static_cast<int>(Chunk::Depth) - 1
            );
        }

        if (z >= static_cast<int>(Chunk::Depth)) {
            if (neighbors.front == nullptr) {
                return BlockType::Air;
            }

            return neighbors.front->block(x, y, 0);
        }

        return BlockType::Air;
    }

    [[nodiscard]] inline bool isFaceVisible(const Chunk &chunk, const ChunkNeighbors &neighbors, int x, int y, int z,
                                            BlockFace face) noexcept {
        int neighborX = x;
        int neighborY = y;
        int neighborZ = z;

        switch (face) {
            case BlockFace::Front:
                ++neighborZ;
                break;

            case BlockFace::Back:
                --neighborZ;
                break;

            case BlockFace::Left:
                --neighborX;
                break;

            case BlockFace::Right:
                ++neighborX;
                break;

            case BlockFace::Top:
                ++neighborY;
                break;

            case BlockFace::Bottom:
                --neighborY;
                break;
        }

        return isAir(
            blockFromNeighborhood(chunk, neighbors, neighborX, neighborY, neighborZ)
        );
    }
} // namespace bedrocked