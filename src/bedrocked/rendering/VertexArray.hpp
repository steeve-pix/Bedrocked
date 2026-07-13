#pragma once

namespace bedrocked {
    class VertexArray final {
    public:
        VertexArray();

        ~VertexArray();

        VertexArray(const VertexArray &) = delete;

        VertexArray &operator=(const VertexArray &) = delete;

        void bind() const noexcept;

    private:
        unsigned int m_id{};
    };
}
