#include "../../include/bedrocked/rendering/Mesh.hpp"
#include "../../include/bedrocked/rendering/Vertex.hpp"

#include <cstddef>
#include <glad/glad.h>

namespace bedrocked {
    Mesh::Mesh(const Vertex *vertices, std::size_t vertexCount, const std::uint32_t *indices, std::size_t indexCount)
        : m_vertexBuffer{vertices, vertexCount * sizeof(Vertex)} {
        m_vertexArray.bind();
        m_vertexBuffer.bind();

        glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void *>(offsetof(Vertex, position)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void *>(offsetof(Vertex, color)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void *>(offsetof(Vertex, textureCoordinates)));
        glEnableVertexAttribArray(2);

        m_indexBuffer.emplace(indices, indexCount);
    }

    void Mesh::bind() const noexcept {
        m_vertexArray.bind();
    }

    int Mesh::indexCount() const noexcept {
        return m_indexBuffer->count();
    }
} // namespace bedrocked
