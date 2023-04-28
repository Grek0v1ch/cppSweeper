#pragma once

#include <string>
#include <map>
#include <memory>

namespace Renderer {
    class ShaderProgram;
    class Texture2D;
    class Sprite;
}

class ResourceManager {
private:
    using ShaderProgramsMap = std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>>;
    using  TexturesMap = std::map<std::string, std::shared_ptr<Renderer::Texture2D>>;
    using SpriteMap = std::map<std::string, std::shared_ptr<Renderer::Sprite>>;

    ShaderProgramsMap _shaders;
    TexturesMap _textures;
    SpriteMap _sprites;
    std::string _resourcePath;

    ResourceManager() = default;

    [[nodiscard]] std::string getFileString(const std::string& filePath) const;
public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    static ResourceManager& instance();
    void setExecutablePath(const std::string& executablePath) noexcept;
    void loadJSONResources(const std::string& JSONPath) noexcept;
    std::shared_ptr<Renderer::ShaderProgram> loadShaderProgram(const std::string& shaderName,
                                                               const std::string& vertexPath,
                                                               const std::string& fragmentPath);
    std::shared_ptr<Renderer::ShaderProgram>
    getShaderProgram(const std::string& shaderName) const noexcept;

    std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName,
                                                     const std::string& filePath);
    std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName) const noexcept;
    std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
                                                 const std::string& textureName,
                                                 const std::string& shaderName,
                                                 unsigned int spriteWidth,
                                                 unsigned int spriteHeight,
                                                 const std::string& subTextureName);
    std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName) noexcept;
    std::shared_ptr<Renderer::Texture2D>
    loadTextureAtlas(const std::string& textureName,
                     const std::string& texturePath,
                     const std::vector<std::string>& subTextures,
                     unsigned int subTextureWidth,
                     unsigned int subTextureHeight);
};
