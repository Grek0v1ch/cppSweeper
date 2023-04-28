#include "Ranges.h"

#include <random>

namespace Game {
    Ranges& Ranges::instance() noexcept {
        static Ranges instance;
        return instance;
    }

    void Ranges::setSize(const Game::Coord size) noexcept {
        _size = size;
        for (int x = 0; x < _size.x; x++) {
            for (int y = 0; y < _size.y; y++) {
                _allCoords.push_back({x, y});
            }
        }
    }

    bool Ranges::inRange(const Game::Coord coord) const noexcept {
        return coord.x >= 0 and coord.x < _size.x and coord.y >= 0 and coord.y < _size.y;
    }

    Coord Ranges::getRandomCoord() const noexcept {
        return {getRandomX(), getRandomY()};
    }

    std::vector<Coord> Ranges::getCoordsAround(const Coord coord) const noexcept {
        std::vector<Coord> result;
        result.reserve(8);
        for (int x = coord.x - 1; x <= coord.x + 1; x++) {
            for (int y = coord.y - 1; y <= coord.y + 1; y++) {
                Coord currCoord {x, y};
                if (inRange(currCoord) and currCoord != coord) {
                    result.push_back(currCoord);
                }
            }
        }
        return result;
    }

    int Ranges::getRandomX() const noexcept {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<int> uniform_dist(0, _size.x - 1);
        return uniform_dist(e1);
    }

    int Ranges::getRandomY() const noexcept {
        std::random_device r;
        std::default_random_engine e1(r());
        std::uniform_int_distribution<int> uniform_dist(0, _size.y - 1);
        return uniform_dist(e1);
    }
}