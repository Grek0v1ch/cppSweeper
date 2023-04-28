#include "Bomb.h"

namespace Game {
    Bomb::Bomb(int totalBomb) noexcept : _totalBomb(totalBomb), _bombMap() {
        fixBombCount();
    }

    void Bomb::start() noexcept {
        _bombMap = GameMap(TypeBox::ZERO);
        for (int i = 0; i < _totalBomb; i++) {
            placeBomb();
        }
    }

    void Bomb::fixBombCount() noexcept {
        Ranges& ranges = Ranges::instance();
        int maxBomb = ranges.getSize().x * ranges.getSize().y / 2;
        if (_totalBomb > maxBomb) {
            _totalBomb = maxBomb;
        }
    }

    void Bomb::placeBomb() noexcept {
        Ranges& ranges = Ranges::instance();
        Coord coord = ranges.getRandomCoord();
        while (_bombMap.get(coord).type() == TypeBox::BOMB) {
            coord = ranges.getRandomCoord();
        }
        _bombMap.set(coord, TypeBox::BOMB);
        incNumbersAroundBomb(coord);
    }

    void Bomb::incNumbersAroundBomb(const Coord coord) noexcept {
        Ranges& ranges = Ranges::instance();
        for (auto currCoord : ranges.getCoordsAround(coord)) {
            if (TypeBox::BOMB != _bombMap.get(currCoord).type()) {
                _bombMap.set(currCoord, _bombMap.get(currCoord).nextNumberBox());
            }
        }
    }

    void Bomb::render() const noexcept {
        _bombMap.render();
    }
}
