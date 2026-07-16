#pragma once

#include <array>
#include <cstddef>

#include "InventorySlot.hpp"
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
            return m_slots[m_selectedSlot].block;
        }

        [[nodiscard]] const InventorySlot &slotAt(std::size_t slot) const noexcept {
            return m_slots[slot];
        }

        [[nodiscard]] std::size_t selectedQuantity() const noexcept {
            return m_slots[m_selectedSlot].quantity;
        }

        [[nodiscard]] bool consumeSelected() noexcept {
            InventorySlot &slot =
                    m_slots[m_selectedSlot];

            if (slot.quantity == 0) {
                return false;
            }

            --slot.quantity;
            return true;
        }

        void add(BlockType type) noexcept {
            for (InventorySlot &slot: m_slots) {
                if (slot.block == type) {
                    ++slot.quantity;
                    return;
                }
            }
        }

    private:
        std::array<InventorySlot, SlotCount> m_slots{
            InventorySlot{BlockType::Grass, 20},
            InventorySlot{BlockType::Dirt, 35},
            InventorySlot{BlockType::Stone, 64},
            InventorySlot{BlockType::Wood, 12},
            InventorySlot{BlockType::Leaves, 18}
        };

        std::size_t m_selectedSlot{2};
    };
} // namespace bedrocked
