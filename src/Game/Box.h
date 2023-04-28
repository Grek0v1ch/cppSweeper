#pragma once

#include <memory>

namespace Renderer {
    class Sprite;
}

namespace Game {
    struct Coord;

    enum class TypeBox : int {
        CLOSED,
        ZERO,
        NUM1,
        NUM2,
        NUM3,
        NUM4,
        NUM5,
        NUM6,
        NUM7,
        NUM8,
        BOMB,
        FLAG,
        NO_BOMB,
        BOMBED,
        INFORM,
        OPEN_INFORM,
        OPENED
    };

    class Box {
    public:
        Box();
        Box(const TypeBox type);
        Box(const Box&) = default;
        Box& operator=(const Box&) = default;

        Box nextNumberBox() const noexcept;
        void render(const Coord coord) const noexcept;

        int getNumber() const noexcept { return static_cast<int>(_type) - 1; }

        void setSprite(const TypeBox type) noexcept;

        TypeBox type() const noexcept { return _type; }

    private:
        TypeBox _type;
        std::shared_ptr<Renderer::Sprite> _sprite;
    };
}