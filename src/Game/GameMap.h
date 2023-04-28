#pragma once

#include <vector>

namespace Game {
    class Box;
    struct Coord;

    class GameMap {
    public:
        GameMap(const GameMap&) = delete;
        GameMap& operator=(const GameMap&) = default;

        GameMap() noexcept;
        GameMap(const Box& defaultBox) noexcept;

        Box get(const Coord coord) const;

        void set(const Coord coord, const Box& box) noexcept;

        void render() const noexcept;
    private:
        template<class T> using Matrix = std::vector<std::vector<T>>;

        Matrix<Box> _map;
    };
}
