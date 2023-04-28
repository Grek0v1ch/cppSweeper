#include "Texture2D.h"

namespace Renderer {
    Texture2D::Texture2D(const GLsizei width, const GLsizei height,
                         const unsigned char *data, const unsigned int channels,
                         const GLint filter, const GLint wrapMode) :
                         _id(0), _width(width), _height(height) {
        GLenum mode = 0;
        switch (channels) {
            case 4:
                mode = GL_RGBA;
                break;
            case 3:
                mode = GL_RGB;
                break;
            default:
                mode = GL_RGBA;
                break;
        }
        glGenTextures(1, &_id);
        glActiveTexture(GL_TEXTURE0);
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, mode, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glGenerateMipmap(GL_TEXTURE_2D);
        unbind();
    }

    Texture2D::~Texture2D() noexcept {
        glDeleteTextures(1, &_id);
    }

    Texture2D::Texture2D(Renderer::Texture2D&& o) noexcept {
        _id = o._id;
        o._id = 0;
        _width = o._width;
        _height = o._height;
    }

    Texture2D& Texture2D::operator=(Renderer::Texture2D&& o) noexcept {
        glDeleteTextures(1, &_id);
        _id = o._id;
        o._id = 0;
        _width = o._width;
        _height = o._height;
        return *this;
    }

    void Texture2D::addSubTexture(const std::string& name, const glm::vec2& leftBottomUV,
                                  const glm::vec2& rightTomUV) {
        _subTextures.emplace(std::move(name), SubTexture2D{leftBottomUV, rightTomUV});
    }

    const Texture2D::SubTexture2D& Texture2D::getSubTexture(const std::string& name) const {
        auto it = _subTextures.find(name);
        if (it != _subTextures.end()) {
            return it->second;
        }
        const static SubTexture2D defaultSubTexture;
        return defaultSubTexture;
    }

    void Texture2D::bind() const noexcept {
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    void Texture2D::unbind() const noexcept {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}