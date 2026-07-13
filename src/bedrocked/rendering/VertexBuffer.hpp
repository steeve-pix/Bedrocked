#pragma once

#include <cstddef>
#include <cstdint>

namespace bedrocked {
    class VertexBuffer final {
    public:
        VertexBuffer(const void *data, std::size_t sizeInBytes);

        ~VertexBuffer();

        VertexBuffer(const VertexBuffer &) = delete;

        VertexBuffer &operator=(const VertexBuffer &) = delete;

        void bind() const noexcept;

    private:
        uint32_t m_id{};
    };
}
