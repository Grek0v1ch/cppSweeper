#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <string>
#include <map>

namespace Renderer {
    class Texture2D {
    public:
        struct SubTexture2D {
            glm::vec2 leftBottomUV {0.0f};
            glm::vec2 rightTopUV {1.0f};
        };

        Texture2D(GLsizei width, GLsizei height,
                  const unsigned char* data, unsigned int channels = 4,
                  GLint filter = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
        ~Texture2D() noexcept;
        Texture2D& operator=(Texture2D&& o) noexcept;
        Texture2D(Texture2D&& o) noexcept;

        void addSubTexture(const std::string& name, const glm::vec2& leftBottomUV,
                           const glm::vec2& rightTomUV);
        const Texture2D::SubTexture2D& getSubTexture(const std::string& name) const;
        void bind() const noexcept;
        void unbind() const noexcept;
        GLsizei width() const noexcept { return _width; }
        GLsizei height() const noexcept { return _height; }

        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;

    private:
        GLuint _id;
        GLsizei _width;
        GLsizei _height;
        std::map<std::string, SubTexture2D> _subTextures;
    };
}