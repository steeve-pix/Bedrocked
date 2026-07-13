#pragma once
#include "bedrocked/rendering/VertexArray.hpp"
#include "bedrocked/rendering/VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include <optional>

#include "bedrocked/rendering/Vertex.hpp"

namespace bedrocked {
    class Mesh final {
    public:
        Mesh(const Vertex *vertices, std::size_t vertexSizeInBytes, const unsigned int *indices, std::size_t indexCount);

        Mesh(const Mesh &) = delete;

        Mesh &operator=(const Mesh &) = delete;

        void bind() const noexcept;

        [[nodiscard]] int indexCount() const noexcept;

    private:
        VertexArray m_vertexArray;
        VertexBuffer m_vertexBuffer;
        std::optional<IndexBuffer> m_indexBuffer;
    };
}
