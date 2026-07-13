#pragma once
#include "bedrocked/rendering/VertexArray.hpp"
#include "bedrocked/rendering/VertexBuffer.hpp"

#include <cstddef>

namespace bedrocked {
    class Mesh final {
    public:
        Mesh(const float *vertices, std::size_t sizeInBytes, int vertexCount);

        Mesh(const Mesh &) = delete;

        Mesh &operator=(const Mesh &) = delete;

        void bind() const noexcept;

        [[nodiscard]] int vertexCount() const noexcept;

    private:
        VertexArray m_vertexArray;
        VertexBuffer m_vertexBuffer;
        int m_vertexCount{};
    };
}
