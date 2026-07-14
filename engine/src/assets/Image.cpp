#include "../../include/bedrocked/assets/Image.hpp"

#include <stdexcept>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace bedrocked {
    Image::Image(const std::string &path) {
        constexpr int requestedChannels = 4;

        int originalChannelCount{};

        stbi_set_flip_vertically_on_load(1);
        stbi_uc *loadedPixels = stbi_load(path.c_str(), &m_width, &m_height, &originalChannelCount, requestedChannels);

        if (loadedPixels == nullptr) {
            const char *reason = stbi_failure_reason();

            throw std::runtime_error{
                "Failed to load image '" + path + "': " + (reason != nullptr ? reason : "unknown error")
            };
        }

        const std::size_t pixelCount =
                static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height) * requestedChannels;

        m_pixels.assign(loadedPixels, loadedPixels + pixelCount);

        stbi_image_free(loadedPixels);
    }

    int Image::width() const noexcept {
        return m_width;
    }

    int Image::height() const noexcept {
        return m_height;
    }

    const std::uint8_t *Image::pixels() const noexcept {
        return m_pixels.data();
    }
} // namespace bedrocked
