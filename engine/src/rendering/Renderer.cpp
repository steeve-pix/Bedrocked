#include "../../include/bedrocked/rendering/Renderer.hpp"

#include <glad/glad.h>

namespace bedrocked {
    Renderer::Renderer() {
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }

    void Renderer::setClearColor(float red, float green, float blue, float alpha) noexcept {
        glClearColor(red, green, blue, alpha);
    }

    void Renderer::clear() noexcept {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw(const Mesh &mesh) noexcept {
        mesh.bind();

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(mesh.indexCount()),
            GL_UNSIGNED_INT,
            nullptr
        );
    }

    void Renderer::setViewPort(int width, int height) noexcept {
        glViewport(0, 0, width, height);
    }
} // namespace bedrocked
