#pragma once

#include <cstddef>
#include <cstdint>

namespace bedrocked {
    class IndexBuffer final {
    public:
        IndexBuffer(const uint32_t *indices, std::size_t count);

        ~IndexBuffer();

        IndexBuffer(const IndexBuffer &) = delete;

        IndexBuffer &operator=(const IndexBuffer &) = delete;

        void bind() const noexcept;

        [[nodiscard]] std::size_t count() const noexcept;

    private:
        uint32_t m_id{0};
        std::size_t m_count{};
    };
} // namespace bedrocked
