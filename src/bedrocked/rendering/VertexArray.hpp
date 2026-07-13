#pragma once
#include <cstdint>

namespace bedrocked {
    class VertexArray final {
    public:
        VertexArray();

        ~VertexArray();

        VertexArray(const VertexArray &) = delete;

        VertexArray &operator=(const VertexArray &) = delete;

        void bind() const noexcept;

    private:
        uint32_t m_id{};
    };
}
