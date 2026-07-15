#pragma once
#include <cstdint>

namespace bedrocked {
    enum class BlockType: std::uint8_t {
        Air,
        Grass,
        Dirt,
        Stone,
        Wood
    };
} // namespace bedrocked
