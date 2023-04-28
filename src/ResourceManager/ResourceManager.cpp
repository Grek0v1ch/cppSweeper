#include "ResourceManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../../external/json.hpp"

using json = nlohmann::json;

#include "../Exception/Exception.h"
#include "../RenderEngine/ShaderProgram.h"
#include "../RenderEngine/Texture2D.h"
#include "../RenderEngine/Sprite.h"

using Renderer::ShaderProgram;
using Renderer::Texture2D;
using Renderer::Sprite;

ResourceManager& ResourceManager::instance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::setExecutablePath(const std::string& executablePath) noexcept {
    size_t found = executablePath.find_last_of("/\\");
    _resourcePath = executablePath.substr(0, found);
}

void ResourceManager::loadJSONResources(const std::string& JSONPath) noexcept {
    json data = json::parse(getFileString(JSONPath));
    if (data.contains("shaders")) {
        for (auto& shader : data["shaders"]) {
            loadShaderProgram(shader["name"],
                              shader["filePath_v"],
                              shader["filePath_f"]);
        }
    }
    if (data.contains("textureAtlases")) {
        for (auto& textureAtlas : data["textureAtlases"]) {
            loadTextureAtlas(textureAtlas["name"],
                             textureAtlas["filePath"],
                             textureAtlas["subTextures"],
                             textureAtlas["subTextureWidth"],
                             textureAtlas["subTextureHeight"]);
        }
    }
    if (data.contains("sprites")) {
        for (auto& sprite : data["sprites"]) {
            loadSprite(sprite["spriteName"],
                       sprite["textureName"],
                       sprite["shaderName"],
                       sprite["spriteWidth"],
                       sprite["spriteHeight"],
                       sprite["subTextureName"]);
        }
    }
}

std::shared_ptr<ShaderProgram>
ResourceManager::loadShaderProgram(const std::string& shaderName,
                                   const std::string& vertexPath,
                                   const std::string& fragmentPath) {
    std::string vertexString = getFileString(vertexPath);
    if (vertexString.empty()) {
        throw Exception::Exception("No vertex shader!");
    }
    std::string fragmentString = getFileString(fragmentPath);
    if (fragmentString.empty()) {
        throw Exception::Exception("No fragment shader!");
    }
    try {
        auto temp = _shaders.emplace(shaderName,
                                     std::make_shared<ShaderProgram>(vertexString, fragmentString));
        return temp.first->second;
    } catch (Exception::Exception& ex) {
        std::string msg = "\nCan't load shader program:\nVertex: ";
        msg += vertexPath + "\nFragment: ";
        msg += fragmentPath + "\n";
        ex.addMsg(msg);
        throw ex;
    }
}

std::shared_ptr<ShaderProgram>
ResourceManager::getShaderProgram(const std::string& shaderName) const noexcept {
    auto tmp = _shaders.find(shaderName);
    if (tmp != _shaders.end()) {
        return tmp->second;
    }
    return nullptr;
}

std::shared_ptr<Texture2D>
ResourceManager::loadTexture(const std::string& textureName, const std::string& filePath) {
    int channels = 0;
    int width = 0;
    int height = 0;
    std::string absolutelyFilePath {_resourcePath + '/' + filePath};
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(absolutelyFilePath.c_str(), &width, &height, &channels, 0);
    if (! pixels) {
        throw Exception::Exception("Can't load image: " + filePath);
    }
    auto tmp = _textures.emplace(textureName, std::make_shared<Texture2D>(width, height, pixels,
                                                                          channels, GL_NEAREST));
    stbi_image_free(pixels);
    return tmp.first->second;
}

std::shared_ptr<Renderer::Texture2D>
ResourceManager::getTexture(const std::string& textureName) const noexcept {
    auto tmp = _textures.find(textureName);
    if (tmp != _textures.end()) {
        return tmp->second;
    }
    return nullptr;
}

std::shared_ptr<Sprite>
ResourceManager::loadSprite(const std::string& spriteName,
                            const std::string& textureName, const std::string& shaderName,
                            const unsigned int spriteWidth, const unsigned int spriteHeight,
                            const std::string& subTextureName) {
    auto pTexture = getTexture(textureName);
    if (! pTexture) {
        throw Exception::Exception("Can't find the texture: " + textureName +
                                   " for the sprite: " + spriteName);
    }

    auto pShaderProgram = getShaderProgram(shaderName);
    if (! pShaderProgram) {
        throw Exception::Exception("Can't find the shader program: " + shaderName +
                                   " for the sprite: " + spriteName);
    }

    auto tmp = _sprites.emplace(spriteName,
                                std::make_shared<Sprite>(pTexture, subTextureName, pShaderProgram,
                                                         glm::vec2(0, 0),
                                                         glm::vec2(spriteWidth, spriteHeight)));
    auto newSprite = tmp.first->second;
    return newSprite;
}

std::shared_ptr<Sprite>
ResourceManager::getSprite(const std::string& spriteName) noexcept {
    auto it = _sprites.find(spriteName);
    if (it != _sprites.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Renderer::Texture2D>
ResourceManager::loadTextureAtlas(const std::string& textureName,
                                  const std::string& texturePath,
                                  const std::vector<std::string>& subTextures,
                                  const unsigned int subTextureWidth,
                                  const unsigned int subTextureHeight) {
    auto pTexture = loadTexture(textureName, texturePath);
    const unsigned int textureWidth = pTexture->width();
    const unsigned int textureHeight = pTexture->height();
    unsigned int currentTextureOffsetX = 0;
    unsigned int currentTextureOffsetY = textureHeight;
    for (const auto& currentSubTextureName : subTextures) {
        glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
                               static_cast<float>(currentTextureOffsetY - subTextureHeight) /
                                   textureHeight);
        glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) /
                                 textureWidth,
                             static_cast<float>(currentTextureOffsetY) / textureHeight);
        pTexture->addSubTexture(currentSubTextureName, leftBottomUV, rightTopUV);
        currentTextureOffsetX += subTextureWidth;
        if (currentTextureOffsetX >= textureWidth) {
            currentTextureOffsetX = 0;
            currentTextureOffsetY -= subTextureHeight;
        }
    }
    return pTexture;
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const {
    std::ifstream fin(_resourcePath + '/' + relativeFilePath);
    if (! fin.is_open()) {
        throw Exception::Exception("Failed to open file: " + relativeFilePath);
    }
    std::stringstream buffer;
    buffer << fin.rdbuf();
    return buffer.str();
}
