#pragma once

namespace bedrocked {

    enum class PauseMenuAction {
        None,
        Resume,
        Exit
    };

    class PauseMenu final {
    public:
        [[nodiscard]] PauseMenuAction draw() const;
    };

} // namespace bedrocked