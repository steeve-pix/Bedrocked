#include "../../../include/bedrocked/rendering/opengl/IndexBuffer.hpp"

#include <glad/glad.h>

namespace bedrocked {
    IndexBuffer::IndexBuffer(const uint32_t *indices, std::size_t count) : m_count{count} {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices,
                     GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() {
        if (m_id != 0) {
            glDeleteBuffers(1, &m_id);
        }
    }

    void IndexBuffer::bind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    std::size_t IndexBuffer::count() const noexcept {
        return m_count;
    }
} // namespace bedrocked
