#pragma once
#include "bedrocked/math/Vector3.hpp"
#include "bedrocked/world/block/BlockPosition.hpp"

namespace bedrocked {
    class World;

    class Player final {
    public:
        explicit Player(Vector3 position) noexcept;

        void update(float deltaTime, const World &world) noexcept;

        [[nodiscard]] const Vector3 &position() const noexcept;

        [[nodiscard]] const Vector3 &velocity() const noexcept;

        void move(float forward, float right, float yaw, float speed) noexcept;

        void jump(float jumpVelocity) noexcept;

        [[nodiscard]] bool intersectsBlock(BlockPosition block) const noexcept;

    private:
        Vector3 m_position;
        Vector3 m_velocity;

        bool m_grounded{};
    };
} // namespace bedrocked
