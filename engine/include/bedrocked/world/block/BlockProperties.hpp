#pragma once
#include "bedrocked/world/BlockType.hpp"

namespace bedrocked {
    [[nodiscard]] constexpr bool isAir(BlockType type) noexcept {
        return type == BlockType::Air;
    }

    [[nodiscard]] constexpr bool isSolid(BlockType type) noexcept {
        switch (type) {
            case BlockType::Air:
                return false;
            case BlockType::Grass:
            case BlockType::Dirt:
            case BlockType::Stone:
            case BlockType::Wood:
                return true;
        }
        return false;
    }
}
