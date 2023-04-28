#include "Box.h"

#include <vector>

#include "../ResourceManager/ResourceManager.h"
#include "../RenderEngine/Sprite.h"
#include "Ranges.h"

namespace Game {
    Box::Box() : _type(TypeBox::CLOSED) {
        setSprite(TypeBox::CLOSED);
    }

    Box::Box(const Game::TypeBox type) : _type(type) {
        setSprite(type);
    }

    void Box::setSprite(const Game::TypeBox type) noexcept {
        if (TypeBox::OPENED == type) {
            _sprite = nullptr;
            return;
        }
        ResourceManager& resourceManager = ResourceManager::instance();
        std::vector<std::string> spriteNames {
             "closed", "zero", "num1", "num2", "num3", "num4", "num5", "num6", "num7",
             "num8", "bomb", "flag", "noBomb", "bombed", "inform", "openInform"
        };
        _sprite = resourceManager.getSprite(spriteNames[static_cast<size_t>(type)]);
    }

    Box Box::nextNumberBox() const noexcept {
        if (_type < TypeBox::ZERO or _type > TypeBox::NUM8) {
            return {TypeBox::OPEN_INFORM};
        }
        int number = static_cast<int>(_type);
        return {static_cast<TypeBox>(number + 1)};
    }

    void Box::render(const Coord coord) const noexcept {
        if (_sprite) {
            _sprite->setPosition({coord.x * _sprite->size().x, coord.y * _sprite->size().y});
            _sprite->render();
        }
    }
}