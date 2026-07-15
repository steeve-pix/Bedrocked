#pragma once

namespace bedrocked {
    struct ChunkPosition {
        int x, y, z;

        [[nodiscard]] constexpr bool operator==(const ChunkPosition &) const noexcept = default;
    };
} // namespace bedrocked
