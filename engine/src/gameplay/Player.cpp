#include "bedrocked/world/World.hpp"
#include "bedrocked/world/block/BlockProperties.hpp"
#include "bedrocked/gameplay/Player.hpp"
#include <cmath>
#include <array>

namespace bedrocked {
    Player::Player(Vector3 position) noexcept
        : m_position{position} {
    }

    void Player::update(float deltaTime, const World &world) noexcept {
        constexpr float gravity = -20.0F;
        constexpr float halfWidth = 0.3F;
        constexpr float collisionEpsilon = 0.001F;

        // Horizontal movement
        m_position.x += m_velocity.x * deltaTime;
        m_position.z += m_velocity.z * deltaTime;

        if (!m_grounded) {
            m_velocity.y += gravity * deltaTime;
        }

        const float proposedY =
                m_position.y + m_velocity.y * deltaTime;

        if (m_velocity.y <= 0.0F) {
            const int blockY = static_cast<int>(
                std::floor(proposedY - collisionEpsilon)
            );

            constexpr std::array horizontalOffsets{
                -halfWidth,
                halfWidth
            };

            bool groundFound = false;

            for (const float offsetX: horizontalOffsets) {
                for (const float offsetZ: horizontalOffsets) {
                    const int blockX = static_cast<int>(
                        std::floor(m_position.x + offsetX)
                    );

                    const int blockZ = static_cast<int>(
                        std::floor(m_position.z + offsetZ)
                    );

                    if (isSolid(world.blockAtWorld(blockX, blockY, blockZ))) {
                        groundFound = true;
                        break;
                    }
                }

                if (groundFound) {
                    break;
                }
            }

            if (groundFound) {
                m_position.y =
                        static_cast<float>(blockY + 1);

                m_velocity.y = 0.0F;
                m_grounded = true;
                return;
            }
        }

        m_position.y = proposedY;
        m_grounded = false;
    }

    const Vector3 &Player::position() const noexcept {
        return m_position;
    }

    const Vector3 &Player::velocity() const noexcept {
        return m_velocity;
    }

    void Player::move(float forward, float right, float yaw, float speed) noexcept {
        const float inputLength =
       std::sqrt(forward * forward + right * right);

        if (inputLength > 1.0F) {
            forward /= inputLength;
            right /= inputLength;
        }

        const float sine = std::sin(yaw);
        const float cosine = std::cos(yaw);

        const float forwardX = -sine;
        const float forwardZ = -cosine;

        const float rightX = cosine;
        const float rightZ = -sine;

        m_velocity.x =
                (forwardX * forward + rightX * right) * speed;

        m_velocity.z =
                (forwardZ * forward + rightZ * right) * speed;
    }
} // namespace bedrocked
