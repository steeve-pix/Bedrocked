#pragma once
#include "../block/BlockType.hpp"
#include <array>
#include <cstddef>

namespace bedrocked {
    class Chunk final {
    public:
        static constexpr std::size_t Width = 16;
        static constexpr std::size_t Height = 16;
        static constexpr std::size_t Depth = 16;

        static constexpr std::size_t BlockCount =
                Width * Height * Depth;


        [[nodiscard]] static constexpr bool contains(int x, int y, int z) noexcept {
            return x >= 0 &&
                   y >= 0 &&
                   z >= 0 &&
                   x < static_cast<int>(Width) &&
                   y < static_cast<int>(Height) &&
                   z < static_cast<int>(Depth);
        }

        [[nodiscard]] BlockType block(int x, int y, int z) const noexcept;

        void setBlock(int x, int y, int z, BlockType type) noexcept;

        [[nodiscard]] BlockType blockOrAir(int x, int y, int z) const noexcept;

    private:
        [[nodiscard]] static constexpr std::size_t index(int x, int y, int z) noexcept {
            const auto arrayX = static_cast<std::size_t>(x);
            const auto arrayY = static_cast<std::size_t>(y);
            const auto arrayZ = static_cast<std::size_t>(z);

            return arrayX + Width * (arrayZ + Depth * arrayY);
        }

        std::array<BlockType, BlockCount> m_blocks{};
    };
}
