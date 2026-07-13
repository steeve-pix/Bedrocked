#include "bedrocked/rendering/Mesh.hpp"

#include <glad/glad.h>

namespace bedrocked {
    Mesh::Mesh(const float *vertices, std::size_t sizeInBytes, int vertexCount)
        : m_vertexBuffer(vertices, sizeInBytes), m_vertexCount(vertexCount) {
        m_vertexArray.bind();
        m_vertexBuffer.bind();

        glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }

    void Mesh::bind() const noexcept {
        m_vertexArray.bind();
    }

    int Mesh::vertexCount() const noexcept {
        return m_vertexCount;
    }
}
