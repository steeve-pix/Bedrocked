#pragma once

#include <array>
#include <cstddef>

#include "bedrocked/world/block/BlockType.hpp"

namespace bedrocked {
    class Hotbar final {
    public:
        static constexpr std::size_t SlotCount = 5;

        void select(std::size_t slot) noexcept {
            if (slot < SlotCount) {
                m_selectedSlot = slot;
            }
        }

        [[nodiscard]] std::size_t selectedSlot() const noexcept {
            return m_selectedSlot;
        }

        [[nodiscard]] BlockType selectedBlock() const noexcept {
            return m_blocks[m_selectedSlot];
        }

        [[nodiscard]] BlockType blockAt(std::size_t slot) const noexcept {
            return m_blocks[slot];
        }

    private:
        std::array<BlockType, SlotCount> m_blocks{
            BlockType::Grass,
            BlockType::Dirt,
            BlockType::Stone,
            BlockType::Wood,
            BlockType::Leaves,
        };

        std::size_t m_selectedSlot{2};
    };
} // namespace bedrocked
