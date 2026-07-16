#include "bedrocked/world/block/BlockRaycast.hpp"

#include "bedrocked/world/World.hpp"
#include "bedrocked/world/block/BlockProperties.hpp"

#include <cmath>

namespace {
    [[nodiscard]] bedrocked::BlockPosition blockContaining(
        const bedrocked::Vector3 &point) noexcept {
        return {
            .x = static_cast<int>(std::floor(point.x)),
            .y = static_cast<int>(std::floor(point.y)),
            .z = static_cast<int>(std::floor(point.z))
        };
    }
} // namespace

namespace bedrocked {
    std::optional<BlockRaycastHit> raycastBlocks(
        const World &world,
        Vector3 origin,
        Vector3 direction,
        float maximumDistance) noexcept {
        const float directionLength =
                std::sqrt(
                    direction.x * direction.x +
                    direction.y * direction.y +
                    direction.z * direction.z
                );

        if (directionLength <= 0.0F ||
            maximumDistance <= 0.0F) {
            return std::nullopt;
        }

        direction.x /= directionLength;
        direction.y /= directionLength;
        direction.z /= directionLength;

        constexpr float stepSize = 0.05F;

        BlockPosition previousBlock =
                blockContaining(origin);

        for (float distance = 0.0F;
             distance <= maximumDistance;
             distance += stepSize) {
            const Vector3 point{
                .x = origin.x + direction.x * distance,
                .y = origin.y + direction.y * distance,
                .z = origin.z + direction.z * distance
            };

            const BlockPosition currentBlock =
                    blockContaining(point);

            if (currentBlock == previousBlock) {
                continue;
            }

            const BlockType type =
                    world.blockAtWorld(
                        currentBlock.x,
                        currentBlock.y,
                        currentBlock.z
                    );

            if (isSolid(type)) {
                return BlockRaycastHit{
                    .block = currentBlock,
                    .adjacentBlock = previousBlock,
                    .distance = distance
                };
            }

            previousBlock = currentBlock;
        }

        return std::nullopt;
    }
}
