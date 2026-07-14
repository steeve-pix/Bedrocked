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
        static constexpr std::size_t BlockCount = Width * Height * Depth;

        [[nodiscard]] BlockType block(std::size_t x, std::size_t y, std::size_t z) const noexcept;

        void setBlock(std::size_t x, std::size_t y, std::size_t z,BlockType type) noexcept;

    private:
        std::array<BlockType, BlockCount> m_blocks{};

        [[nodiscard]] static constexpr std::size_t index(std::size_t x, std::size_t y, std::size_t z) noexcept {
            return x + Width * (z + Depth * y);
        }
    };
}
