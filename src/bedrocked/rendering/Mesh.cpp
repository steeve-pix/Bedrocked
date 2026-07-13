#include "bedrocked/rendering/Mesh.hpp"

#include <glad/glad.h>

namespace bedrocked {
    Mesh::Mesh(const float *vertices, std::size_t vertexSizeInBytes, const unsigned int *indices,
               std::size_t indexCount)
        : m_vertexBuffer{vertices, vertexSizeInBytes} {
        m_vertexArray.bind();
        m_vertexBuffer.bind();

        glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        m_indexBuffer.emplace(indices, indexCount);
    }

    void Mesh::bind() const noexcept {
        m_vertexArray.bind();
    }

    int Mesh::indexCount() const noexcept {
        return m_indexBuffer->count();
    }
}
