#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace bedrocked {
    class Image final {
    public:
        explicit Image(const std::string &path);

        [[nodiscard]] int width() const noexcept;

        [[nodiscard]] int height() const noexcept;

        [[nodiscard]] const std::uint8_t *pixels() const noexcept;

    private:
        int m_width{};
        int m_height{};
        std::vector<std::uint8_t> m_pixels;
    };
} // namespace bedrocked
