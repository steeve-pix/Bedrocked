#pragma once
#include "chunk/ChunkManager.hpp"

namespace bedrocked {
    class World final {
    public:
        void generateTestWorld();

        [[nodiscard]] ChunkManager &chunks() noexcept;

        [[nodiscard]] const ChunkManager &chunks() const noexcept;

    private:
        ChunkManager m_chunkManager;
    };
} // namespace bedrocked
