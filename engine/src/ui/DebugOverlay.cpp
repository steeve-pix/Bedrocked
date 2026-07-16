#include "bedrocked/world/block/BlockType.hpp"
#include "bedrocked/world/block/BlockPosition.hpp"
#include "bedrocked/ui/DebugOverlay.hpp"


#include <imgui.h>

namespace {

[[nodiscard]] constexpr const char* blockName(
    bedrocked::BlockType type) noexcept
{
    switch (type) {
        case bedrocked::BlockType::Air:
            return "Air";

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
    }

    return "Unknown";
}

} // namespace

namespace bedrocked {

void DebugOverlay::draw(
    const DebugOverlayData& data) const
{
    const ImGuiViewport* viewport =
        ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(
        ImVec2{
            viewport->WorkPos.x + 12.0F,
            viewport->WorkPos.y + 12.0F
        },
        ImGuiCond_Always
    );

    ImGui::SetNextWindowBgAlpha(0.75F);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoInputs;

    ImGui::Begin(
        "Debug Overlay",
        nullptr,
        flags
    );

    ImGui::Text(
        "FPS: %.1f",
        data.framesPerSecond
    );

    ImGui::Text(
        "Frame time: %.3f ms",
        data.frameTimeMilliseconds
    );

    ImGui::Separator();

    ImGui::Text(
        "Position: %.2f, %.2f, %.2f",
        data.playerPosition.x,
        data.playerPosition.y,
        data.playerPosition.z
    );

    ImGui::Text(
        "Yaw: %.3f",
        data.cameraYaw
    );

    ImGui::Text(
        "Pitch: %.3f",
        data.cameraPitch
    );

    ImGui::Separator();

    ImGui::Text(
        "Selected: %s",
        blockName(data.selectedBlock)
    );

    if (data.targetedBlock.has_value()) {
        const BlockPosition& block =
            data.targetedBlock.value();

        ImGui::Text(
            "Target: %d, %d, %d",
            block.x,
            block.y,
            block.z
        );
    }
    else {
        ImGui::TextUnformatted(
            "Target: none"
        );
    }

    ImGui::Text(
        "Loaded chunks: %zu",
        data.loadedChunkCount
    );

    ImGui::End();
}

} // namespace bedrocked