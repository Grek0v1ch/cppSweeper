#pragma once

#include <vector>

namespace Game {
    struct Coord {
        int x;
        int y;

        bool operator==(const Coord& o) { return x == o.x and y == o.y; }
        bool operator!=(const Coord& o) { return not (*this == o); }
    };

    class Ranges {
    public:
        Ranges(const Ranges&) = delete;
        Ranges& operator=(const Ranges&) = delete;
        Ranges(Ranges&&) = delete;
        Ranges& operator=(Ranges&&) = delete;

        static Ranges& instance() noexcept;

        Coord getSize() const noexcept { return _size; }
        const std::vector<Coord>& getAllCoord() const noexcept { return _allCoords; }
        void setSize(const Coord size) noexcept;
        bool inRange(const Coord coord) const noexcept;
        Coord getRandomCoord() const noexcept;
        std::vector<Coord> getCoordsAround(const Coord coord) const noexcept;
    private:
        Ranges() = default;

        int getRandomX() const noexcept;
        int getRandomY() const noexcept;

        Coord _size;
        std::vector<Coord> _allCoords;
    };

}
