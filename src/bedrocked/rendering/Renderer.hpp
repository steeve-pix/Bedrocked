#pragma once
namespace bedrocked {
    class Renderer final {
    public:
        void setClearColor(float red, float green, float blue, float alpha) noexcept;

        void clear() noexcept;
    };
}
