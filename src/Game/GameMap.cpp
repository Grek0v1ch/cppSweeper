#include "GameMap.h"

#include "Box.h"
#include "Ranges.h"
#include "../Exception/Exception.h"

namespace Game {
    GameMap::GameMap() noexcept {
        Ranges& ranges = Ranges::instance();
        _map = Matrix<Box>(ranges.getSize().x, std::vector<Box>(ranges.getSize().y, Box()));
    }

    GameMap::GameMap(const Box& defaultBox) noexcept {
        Ranges& ranges = Ranges::instance();
        _map = Matrix<Box>(ranges.getSize().x, std::vector<Box>(ranges.getSize().y, defaultBox));

    }

    Box GameMap::get(const Coord coord) const {
        Ranges& ranges = Ranges::instance();
        if (ranges.inRange(coord)) {
            return _map[coord.x][coord.y];
        }
        throw Exception::Exception("Coord not in range!\n");
    }

    void GameMap::set(const Coord coord, const Box& box) noexcept {
        Ranges& ranges = Ranges::instance();
        if (ranges.inRange(coord)) {
            _map[coord.x][coord.y] = box;
        }
    }

    void GameMap::render() const noexcept {
        for (int i = 0; i < _map.size(); i++) {
            for (int j = 0; j < _map[i].size(); j++) {
                _map[i][j].render({i, j});
            }
        }
    }
}