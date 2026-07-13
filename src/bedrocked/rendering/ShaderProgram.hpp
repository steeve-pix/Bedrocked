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

        void setVec3(std::string_view name, float x, float y, float z) const;

        void setMat4(std::string_view name, const float *values) const;

    private:
        uint32_t m_id{0};
    };
}
