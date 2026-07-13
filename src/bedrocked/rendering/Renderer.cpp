#include "bedrocked/rendering/Renderer.hpp"

#include <glad/glad.h>

namespace bedrocked {
    void Renderer::setClearColor(float red, float green, float blue, float alpha) noexcept {
        glClearColor(red, green, blue, alpha);
    }

    void Renderer::clear() noexcept {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::draw(const Mesh &mesh) noexcept {
        mesh.bind();

        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount());
    }
}
