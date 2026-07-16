#pragma once

#include "bedrocked/math/Vector3.hpp"

#include <cstddef>
#include <optional>

namespace bedrocked {

    struct BlockPosition;
    enum class BlockType : unsigned char;

    struct DebugOverlayData {
        double framesPerSecond{};
        double frameTimeMilliseconds{};

        Vector3 playerPosition;

        float cameraYaw{};
        float cameraPitch{};

        BlockType selectedBlock;
        std::optional<BlockPosition> targetedBlock;

        std::size_t loadedChunkCount{};
    };

    class DebugOverlay final {
    public:
        void draw(const DebugOverlayData& data) const;
    };

} // namespace bedrocked