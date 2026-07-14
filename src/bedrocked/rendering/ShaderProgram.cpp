#include "bedrocked/rendering/ShaderProgram.hpp"

#include <string>
#include <stdexcept>
#include <glad/glad.h>

namespace {
    [[nodiscard]] GLuint compileShader(GLuint shaderType, std::string_view source) {
        const GLuint shader = glCreateShader(shaderType);
        if (shader == 0) throw std::runtime_error("Failed to create shder object.");

        // Pass GLSL source to OpenGL
        const char *sourceData = source.data();
        const int sourceLength = static_cast<int>(source.size());

        glShaderSource(shader, 1, &sourceData, &sourceLength);
        glCompileShader(shader);

        // Check GL_COMPILE_STATUS
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_TRUE) {
            return shader;
        }

        int logLength{};
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH, &logLength);

        std::string errorLog(
            static_cast<std::size_t>(logLength),
            '\0'
        );

        glGetShaderInfoLog(
            shader,
            logLength,
            nullptr,
            errorLog.data()
        );

        glDeleteShader(shader);

        throw std::runtime_error{
            "Shader compilation failed:\n" + errorLog
        };
    }
} // Anonymouse helper

namespace bedrocked {
    ShaderProgram::ShaderProgram(std::string_view vertexSource, std::string_view fragmentSource) {
        // 1. Compile vertex shader
        const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader{};

        try {
            fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        } catch (...) {
            glDeleteShader(vertexShader);
            throw;
        }

        m_id = glCreateProgram();

        glAttachShader(m_id, vertexShader);
        glAttachShader(m_id, fragmentShader);

        glLinkProgram(m_id);

        int linkingSucceed{};
        glGetProgramiv(m_id,GL_LINK_STATUS, &linkingSucceed);

        if (linkingSucceed == GL_TRUE) {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return;
        }

        int logLength{};
        glGetProgramiv(m_id,GL_INFO_LOG_LENGTH, &logLength);

        std::string errorLog(
            static_cast<std::size_t>(logLength),
            '\0'
        );

        glGetProgramInfoLog(
            m_id,
            logLength,
            nullptr,
            errorLog.data()
        );

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_id);

        m_id = 0;

        throw std::runtime_error{
            "Shader program linking failed:\n" + errorLog
        };
    }

    ShaderProgram::~ShaderProgram() {
        if (m_id != 0) {
            glDeleteProgram(m_id);
        }
    }

    void ShaderProgram::use() const noexcept {
        glUseProgram(m_id);
    }

    void ShaderProgram::setVec3(std::string_view name, float x, float y, float z) const {
        const std::string uniformName{name};
        const int location = glGetUniformLocation(m_id, uniformName.c_str());

        glUniform3f(location, x, y, z);
    }

    void ShaderProgram::setMat4(std::string_view name, const float *values) const {
        const std::string uniformName{name};

        const int location = glGetUniformLocation(m_id, uniformName.c_str());

        glUniformMatrix4fv(location, 1,GL_FALSE, values);
    }

    void ShaderProgram::setInt(std::string_view name, int value) const {
        const std::string uniformName{name};

        const int location =
                glGetUniformLocation(m_id, uniformName.c_str());

        glUniform1i(location, value);
    }
} // namespace bedrocked
