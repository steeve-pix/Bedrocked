#include "bedrocked/world/World.hpp"
#include "bedrocked/world/block/BlockProperties.hpp"
#include "bedrocked/gameplay/Player.hpp"
#include <cmath>
#include <array>

namespace {
    constexpr float kPlayerHalfWidth = 0.3F;
    constexpr float kPlayerHeight = 1.8F;
    constexpr float kCollisionEpsilon = 0.001F;

    [[nodiscard]] bool collidesWithWorld(const bedrocked::World &world,
                                         const bedrocked::Vector3 &feetPosition) noexcept {
        const int minimumX = static_cast<int>(std::floor(feetPosition.x - kPlayerHalfWidth + kCollisionEpsilon));

        const int maximumX = static_cast<int>(std::floor(feetPosition.x + kPlayerHalfWidth - kCollisionEpsilon));

        const int minimumY = static_cast<int>(std::floor(feetPosition.y + kCollisionEpsilon));

        const int maximumY = static_cast<int>(std::floor(feetPosition.y + kPlayerHeight - kCollisionEpsilon));

        const int minimumZ = static_cast<int>(std::floor(feetPosition.z - kPlayerHalfWidth + kCollisionEpsilon));

        const int maximumZ = static_cast<int>(
            std::floor(feetPosition.z + kPlayerHalfWidth - kCollisionEpsilon));

        for (int y = minimumY; y <= maximumY; ++y) {
            for (int z = minimumZ; z <= maximumZ; ++z) {
                for (int x = minimumX; x <= maximumX; ++x) {
                    if (bedrocked::isSolid(
                        world.blockAtWorld(x, y, z))) {
                        return true;
                    }
                }
            }
        }

        return false;
    }
}

namespace bedrocked {
    Player::Player(Vector3 position) noexcept
        : m_position{position} {
    }

    void Player::update(float deltaTime, const World &world) noexcept {
        constexpr float gravity = -20.0F;
        constexpr float halfWidth = 0.3F;
        constexpr float collisionEpsilon = 0.001F;

        // Horizontal movement
        Vector3 proposedPosition = m_position;

        proposedPosition.x +=
                m_velocity.x * deltaTime;

        if (!collidesWithWorld(world, proposedPosition)) {
            m_position.x = proposedPosition.x;
        } else {
            m_velocity.x = 0.0F;
        }

        proposedPosition = m_position;

        proposedPosition.z +=
                m_velocity.z * deltaTime;

        if (!collidesWithWorld(world, proposedPosition)) {
            m_position.z = proposedPosition.z;
        } else {
            m_velocity.z = 0.0F;
        }

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

    void Player::jump(float jumpVelocity) noexcept {
        if (!m_grounded) {
            return;
        }

        m_velocity.y = jumpVelocity;
        m_grounded = false;
    }
} // namespace bedrocked
