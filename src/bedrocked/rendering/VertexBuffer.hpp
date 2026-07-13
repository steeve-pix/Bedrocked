#pragma once

#include <cstddef>

namespace bedrocked {
    class VertexBuffer final {
    public:
        VertexBuffer(const float *data, std::size_t sizeInBytes);

        ~VertexBuffer();

        VertexBuffer(const VertexBuffer &) = delete;

        VertexBuffer &operator=(const VertexBuffer &) = delete;

        void bind() const noexcept;

    private:
        unsigned int m_id{};
    };
}
