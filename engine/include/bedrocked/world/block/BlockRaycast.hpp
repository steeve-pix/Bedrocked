#pragma once

#include "bedrocked/math/Vector3.hpp"
#include "bedrocked/world/block/BlockPosition.hpp"

#include <optional>

namespace bedrocked {

    class World;

    struct BlockRaycastHit {
        BlockPosition block;
        BlockPosition adjacentBlock;
        float distance;
    };

    [[nodiscard]] std::optional<BlockRaycastHit> raycastBlocks(
        const World& world,
        Vector3 origin,
        Vector3 direction,
        float maximumDistance) noexcept;

} // namespace bedrocked