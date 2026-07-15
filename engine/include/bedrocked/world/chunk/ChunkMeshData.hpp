#pragma once
#include "bedrocked/rendering/Vertex.hpp"

#include <cstdint>
#include <vector>
#include <cstddef>

namespace bedrocked {
    struct ChunkMeshData {
        std::vector<Vertex> vertices;
        std::vector<std::uint32_t> indices;

        [[nodiscard]] bool empty() const noexcept {
            return indices.empty();
        }

        [[nodiscard]] std::size_t vertexCount() const noexcept {
            return vertices.size();
        }

        [[nodiscard]] std::size_t indexCount() const noexcept {
            return indices.size();
        }
    };
} // namespace bedrocked
