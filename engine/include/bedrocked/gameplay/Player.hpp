#pragma once
#include "bedrocked/math/Vector3.hpp"

namespace bedrocked {
    class World;

    class Player final {
    public:
        explicit Player(Vector3 position) noexcept;

        void update(float deltaTime, const World &world) noexcept;

        [[nodiscard]] const Vector3 &position() const noexcept;

        [[nodiscard]] const Vector3 &velocity() const noexcept;

    private:
        Vector3 m_position;
        Vector3 m_velocity;

        bool m_grounded{};
    };
} // namespace bedrocked
