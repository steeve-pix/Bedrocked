#pragma once
#include <cstdint>

namespace bedrocked {
    class Texture2D final {
    public:
        Texture2D(int width, int height, const uint8_t *pixels);

        ~Texture2D();

        Texture2D(const Texture2D &) = delete;

        Texture2D &operator=(const Texture2D &) = delete;

        void bind() const noexcept;

    private:
        unsigned int m_id{0};
    };
} // bedrocked
