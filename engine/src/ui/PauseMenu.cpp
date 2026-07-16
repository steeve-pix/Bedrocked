#include "bedrocked/ui/PauseMenu.hpp"

#include <imgui.h>

namespace bedrocked {

PauseMenuAction PauseMenu::draw() const
{
    const ImGuiViewport* viewport =
        ImGui::GetMainViewport();

    // Darken the game behind the menu.
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        viewport->WorkPos,
        ImVec2{
            viewport->WorkPos.x + viewport->WorkSize.x,
            viewport->WorkPos.y + viewport->WorkSize.y
        },
        IM_COL32(0, 0, 0, 140)
    );

    constexpr float menuWidth = 320.0F;
    constexpr float menuHeight = 190.0F;
    constexpr float buttonWidth = 220.0F;
    constexpr float buttonHeight = 42.0F;

    ImGui::SetNextWindowPos(
        ImVec2{
            viewport->WorkPos.x +
                viewport->WorkSize.x * 0.5F,

            viewport->WorkPos.y +
                viewport->WorkSize.y * 0.5F
        },
        ImGuiCond_Always,
        ImVec2{0.5F, 0.5F}
    );

    ImGui::SetNextWindowSize(
        ImVec2{menuWidth, menuHeight},
        ImGuiCond_Always
    );

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings;

    PauseMenuAction action =
        PauseMenuAction::None;

    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowRounding,
        8.0F
    );

    ImGui::PushStyleVar(
        ImGuiStyleVar_WindowPadding,
        ImVec2{24.0F, 20.0F}
    );

    ImGui::Begin(
        "Pause Menu",
        nullptr,
        flags
    );

    const char* title = "Paused";

    const float titleWidth =
        ImGui::CalcTextSize(title).x;

    ImGui::SetCursorPosX(
        (menuWidth - titleWidth) * 0.5F
    );

    ImGui::TextUnformatted(title);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::SetCursorPosX(
        (menuWidth - buttonWidth) * 0.5F
    );

    if (ImGui::Button(
            "Resume",
            ImVec2{buttonWidth, buttonHeight})) {
        action = PauseMenuAction::Resume;
    }

    ImGui::Spacing();

    ImGui::SetCursorPosX(
        (menuWidth - buttonWidth) * 0.5F
    );

    if (ImGui::Button(
            "Exit",
            ImVec2{buttonWidth, buttonHeight})) {
        action = PauseMenuAction::Exit;
    }

    ImGui::End();

    ImGui::PopStyleVar(2);

    return action;
}

} // namespace bedrocked