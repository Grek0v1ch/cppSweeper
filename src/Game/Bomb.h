#pragma once

#include "GameMap.h"
#include "Ranges.h"
#include "Box.h"

namespace Game {
    class Bomb {
    public:
        Bomb() = delete;
        Bomb& operator=(const Bomb&) = delete;

        Bomb(int totalBomb) noexcept;

        void start() noexcept;
        void render() const noexcept;

        Box get(Coord coord) const noexcept { return _bombMap.get(coord); };
        int getTotalBombs() const noexcept { return _totalBomb; }

    private:
        void fixBombCount() noexcept;
        void placeBomb() noexcept;
        void incNumbersAroundBomb(const Coord coord) noexcept;

        GameMap _bombMap;
        int _totalBomb;
    };
}
