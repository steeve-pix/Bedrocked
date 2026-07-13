#pragma once
#include <string_view>

namespace bedrocked {
    class ShaderProgram final {
    public:
        ShaderProgram(std::string_view vertexSource, std::string_view fragmentSource);

        ~ShaderProgram();

        ShaderProgram(const ShaderProgram &) = delete;

        ShaderProgram &operator=(const ShaderProgram &) = delete;

        void use() const noexcept;

    private:
        unsigned int m_id{0};
    };
}
