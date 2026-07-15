#include "../../../include/bedrocked/rendering/opengl/VertexArray.hpp"

#include <glad/glad.h>

namespace bedrocked {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray() {
        if (m_id != 0) {
            glDeleteVertexArrays(1, &m_id);
        }
    }

    void VertexArray::bind() const noexcept {
        glBindVertexArray(m_id);
    }
} // namespace bedrocked
