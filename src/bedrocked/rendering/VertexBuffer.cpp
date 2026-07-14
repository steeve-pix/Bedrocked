#include "bedrocked/rendering/VertexBuffer.hpp"

#include <glad/glad.h>

namespace bedrocked {
    VertexBuffer::VertexBuffer(const void *data, std::size_t sizeInBytes) {
        glGenBuffers(1, &m_id);

        glBindBuffer(GL_ARRAY_BUFFER, m_id);

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeInBytes), data,GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        if (m_id != 0) { glDeleteBuffers(1, &m_id); }
    }

    void VertexBuffer::bind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }
} // namespace bedrocked
