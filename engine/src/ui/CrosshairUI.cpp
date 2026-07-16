#include "bedrocked/ui/CrosshairUI.hpp"

#include <imgui.h>

namespace bedrocked {

    void CrosshairUI::draw() const
    {
        const ImGuiViewport* viewport =
            ImGui::GetMainViewport();

        const ImVec2 center{
            viewport->WorkPos.x +
                viewport->WorkSize.x * 0.5F,

            viewport->WorkPos.y +
                viewport->WorkSize.y * 0.5F
        };

        constexpr float halfLength = 6.0F;
        constexpr float gap = 2.0F;
        constexpr float thickness = 2.0F;

        ImDrawList* drawList =
            ImGui::GetForegroundDrawList();

        constexpr ImU32 color =
            IM_COL32(255, 255, 255, 230);

        // Horizontal pieces.
        drawList->AddLine(
            ImVec2{center.x - halfLength, center.y},
            ImVec2{center.x - gap, center.y},
            color,
            thickness
        );

        drawList->AddLine(
            ImVec2{center.x + gap, center.y},
            ImVec2{center.x + halfLength, center.y},
            color,
            thickness
        );

        // Vertical pieces.
        drawList->AddLine(
            ImVec2{center.x, center.y - halfLength},
            ImVec2{center.x, center.y - gap},
            color,
            thickness
        );

        drawList->AddLine(
            ImVec2{center.x, center.y + gap},
            ImVec2{center.x, center.y + halfLength},
            color,
            thickness
        );
    }

} // namespace bedrocked