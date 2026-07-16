#pragma once
namespace bedrocked {
    struct BlockPosition {
        int x{}, y{}, z{};

        [[nodiscard]] constexpr bool operator==(const BlockPosition &) const noexcept = default;
    };
}
