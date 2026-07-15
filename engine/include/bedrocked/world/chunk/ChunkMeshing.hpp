#pragma once

#include "bedrocked/world/block/BlockProperties.hpp"
#include "bedrocked/world/chunk/BlockFace.hpp"
#include "bedrocked/world/chunk/Chunk.hpp"

namespace bedrocked {
    [[nodiscard]] inline bool isFaceVisible(const Chunk &chunk, int x, int y, int z, BlockFace face) noexcept {
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
            chunk.blockOrAir(
                neighborX,
                neighborY,
                neighborZ
            )
        );
    }

} // namespace bedrocked