#include "Texture2D.hpp"

#include <glad/glad.h>

namespace bedrocked {
    Texture2D::Texture2D(int width, int height, const uint8_t *pixels) {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }

    Texture2D::~Texture2D() {
        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
        }
    }

    void Texture2D::bind() const noexcept {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
} // namespace bedrocked
