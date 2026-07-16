#pragma once

namespace bedrocked {
    class Hotbar;

    class HotbarUI final {
    public:
        void draw(const Hotbar &hotbar) const;
    };
} // namespace bedrocked
