#pragma once
#include <cstddef>

#include "bedrocked/rendering/TextureRegion.hpp"

namespace bedrocked {
    class TextureAtlasLayout final {
    public:
        constexpr TextureAtlasLayout(std::size_t columns, std::size_t rows) noexcept
            : m_columns(columns), m_rows(rows) {
        }

        [[nodiscard]] constexpr TextureRegion region(
            std::size_t column, std::size_t row) const noexcept {
            const float tileWidth = 1.0F / static_cast<float>(m_columns);
            const float tileHeight = 1.0F / static_cast<float>(m_rows);
            const float minimumU = static_cast<float>(column) * tileWidth;
            const float minimumV = static_cast<float>(row) * tileHeight;

            return TextureRegion{
                .minimumU = minimumU,
                .minimumV = minimumV,
                .maximumU = minimumU + tileWidth,
                .maximumV = minimumV + tileHeight
            };
        }

    private:
        std::size_t m_columns;
        std::size_t m_rows;
    };
} // namespace bedrocked
