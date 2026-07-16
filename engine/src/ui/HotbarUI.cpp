#include "bedrocked/ui/HotbarUI.hpp"

#include "bedrocked/gameplay/Hotbar.hpp"
#include "bedrocked/world/block/BlockType.hpp"
#include "bedrocked/gameplay/InventorySlot.hpp"

#include <cstddef>
#include <cstdio>

#include <imgui.h>

namespace {
    [[nodiscard]] constexpr const char *blockName(
        bedrocked::BlockType type) noexcept {
        switch (type) {
            case bedrocked::BlockType::Grass:
                return "Grass";

            case bedrocked::BlockType::Dirt:
                return "Dirt";

            case bedrocked::BlockType::Stone:
                return "Stone";

            case bedrocked::BlockType::Wood:
                return "Wood";

            case bedrocked::BlockType::Leaves:
                return "Leaves";

            case bedrocked::BlockType::Air:
                return "Air";
        }

        return "Unknown";
    }
} // namespace

namespace bedrocked {
    void HotbarUI::draw(const Hotbar &hotbar) const {
        constexpr float slotWidth = 78.0F;
        constexpr float slotHeight = 70.0F;
        constexpr float slotSpacing = 6.0F;
        constexpr float windowPadding = 10.0F;

        const float hotbarWidth =
                static_cast<float>(Hotbar::SlotCount) * slotWidth +
                static_cast<float>(Hotbar::SlotCount - 1) * slotSpacing +
                windowPadding * 2.0F;

        const float hotbarHeight =
                slotHeight + windowPadding * 2.0F;

        const ImGuiViewport *viewport =
                ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(
            ImVec2{
                viewport->WorkPos.x +
                viewport->WorkSize.x * 0.5F,

                viewport->WorkPos.y +
                viewport->WorkSize.y -
                20.0F
            },
            ImGuiCond_Always,
            ImVec2{0.5F, 1.0F}
        );

        ImGui::SetNextWindowSize(
            ImVec2{hotbarWidth, hotbarHeight},
            ImGuiCond_Always
        );

        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                           ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;

        ImGui::PushStyleVar(
            ImGuiStyleVar_WindowPadding,
            ImVec2{windowPadding, windowPadding}
        );

        ImGui::SetNextWindowBgAlpha(0.75F);
        ImGui::Begin("Block Hotbar", nullptr, flags);

        for (std::size_t slot = 0; slot < Hotbar::SlotCount; ++slot) {
            if (slot > 0) {
                ImGui::SameLine(0.0F, slotSpacing);
            }

            const bool selected =
                    slot == hotbar.selectedSlot();

            if (selected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.85F, 0.65F, 0.15F, 1.0F});
            }

            const InventorySlot &inventorySlot =
                    hotbar.slotAt(slot);

            char label[32]{};

            std::snprintf(label, sizeof(label), "%zu\n%s", slot + 1, blockName(inventorySlot.block));

            ImGui::Button(label, ImVec2{slotWidth, slotHeight});

            if (selected) {
                ImGui::PopStyleColor();
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }
} // namespace bedrocked
