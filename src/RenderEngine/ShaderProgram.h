#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <string>

namespace Renderer {
    class ShaderProgram {
    public:
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;


        ShaderProgram(const std::string&, const std::string&);
        ShaderProgram(Renderer::ShaderProgram&&) noexcept;
        ~ShaderProgram();

        void use() const;
        void setUniform(const std::string&, const glm::mat4&);
        void setUniform(const std::string&, const GLint value);
    private:
        GLuint program_ID;

        static GLuint createShader(const std::string&, GLenum);
    };
}