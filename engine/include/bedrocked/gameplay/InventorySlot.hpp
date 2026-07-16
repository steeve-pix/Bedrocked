#pragma once

#include "bedrocked/world/block/BlockType.hpp"

#include <cstddef>

namespace bedrocked {

    struct InventorySlot {
        BlockType block{BlockType::Air};
        std::size_t quantity{};
    };

} // namespace bedrocked