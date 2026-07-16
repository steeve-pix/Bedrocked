#pragma once
#include <cstdint>

namespace bedrocked {
    [[nodiscard]] inline bool shouldPlaceTree(int worldX, int worldZ, std::uint32_t seed) noexcept {
        std::uint32_t hash = seed;

        hash ^= static_cast<std::uint32_t>(worldX) * 0x27D4EB2DU;
        hash ^= static_cast<std::uint32_t>(worldZ) * 0x165667B1U;

        hash ^= hash >> 15U;
        hash *= 0x85EBCA6BU;
        hash ^= hash >> 13U;

        constexpr std::uint32_t treeChance = 80U;

        return hash % treeChance == 0U;
    }
} // namespace bedrocked
