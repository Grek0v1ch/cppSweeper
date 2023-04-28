#pragma once

#include "GameMap.h"
#include "Ranges.h"
#include "Box.h"

namespace Game {
    class Flag {
    public:
        Flag(const Flag&) = delete;
        Flag& operator=(const Flag&) = delete;

        Flag();

        void start();
        void toggleFlaggedToBox(Coord coord) noexcept;
        void render() const noexcept;

        Box get(Coord coord) const noexcept;
        int getCountClosedBox() const noexcept { return _countClosedBox; }
        int getCountFlaggedBox() const noexcept { return _countFlagBox; }
        int getCountFlaggedBoxesAround(Coord coord) const noexcept;

        void setCloseBox(Coord coord) noexcept;
        void setFlagToBox(Coord coord) noexcept;
        void setBombedToBox(Coord coord) noexcept;
        void setOpenedToBox(const Coord coord) noexcept;
        void setOpenedToClosedBox(Coord coord) noexcept;
        void setNoBombToFlagSaveBox(Coord coord) noexcept;

    private:
        GameMap _flagMap;
        int _countClosedBox;
        int _countFlagBox;
    };

}
