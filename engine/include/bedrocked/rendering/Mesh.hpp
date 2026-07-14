#pragma once
#include "opengl/VertexArray.hpp"
#include "opengl/IndexBuffer.hpp"
#include "opengl/VertexBuffer.hpp"
#include "Vertex.hpp"

#include <optional>


namespace bedrocked {
    class Mesh final {
    public:
        Mesh(const Vertex *vertices, std::size_t vertexCount, const std::uint32_t *indices, std::size_t);

        Mesh(const Mesh &) = delete;

        Mesh &operator=(const Mesh &) = delete;

        void bind() const noexcept;

        [[nodiscard]] int indexCount() const noexcept;

    private:
        VertexArray m_vertexArray;
        VertexBuffer m_vertexBuffer;
        std::optional<IndexBuffer> m_indexBuffer;
    };
} // namespace bedrocked
