#include "Flag.h"

#include "Ranges.h"

namespace Game {
    Flag::Flag() : _flagMap(), _countClosedBox(), _countFlagBox() {}

    void Flag::start() {
        Ranges& ranges = Ranges::instance();
        _flagMap = GameMap(TypeBox::CLOSED);
        _countClosedBox = ranges.getSize().x * ranges.getSize().y;
        _countFlagBox = 0;
    }

    void Flag::toggleFlaggedToBox(Game::Coord coord) noexcept {
        switch (_flagMap.get(coord).type()) {
            case TypeBox::CLOSED:
                setFlagToBox(coord);
                break;
            case TypeBox::FLAG:
                setCloseBox(coord);
                break;
            default:
                break;
        }
    }

    void Flag::render() const noexcept {
        _flagMap.render();
    }

    Box Flag::get(Game::Coord coord) const noexcept {
        return _flagMap.get(coord);
    }

    int Flag::getCountFlaggedBoxesAround(Game::Coord coord) const noexcept {
        Ranges& ranges = Ranges::instance();
        int count = 0;
        for (const auto& currCoord : ranges.getCoordsAround(coord)) {
            if (_flagMap.get(coord).type() == TypeBox::FLAG) {
                count++;
            }
        }
        return count;
    }

    void Flag::setCloseBox(Game::Coord coord) noexcept {
        if (_flagMap.get(coord).type() == TypeBox::FLAG) {
            _countFlagBox--;
        }
        _flagMap.set(coord, TypeBox::CLOSED);
    }

    void Flag::setFlagToBox(Game::Coord coord) noexcept {
        _flagMap.set(coord, TypeBox::FLAG);
        _countFlagBox++;
    }

    void Flag::setBombedToBox(Game::Coord coord) noexcept {
        _flagMap.set(coord, TypeBox::BOMBED);
    }

    void Flag::setOpenedToBox(const Game::Coord coord) noexcept {
        _flagMap.set(coord, TypeBox::OPENED);
        _countClosedBox--;
    }

    void Flag::setOpenedToClosedBox(Game::Coord coord) noexcept {
        if (_flagMap.get(coord).type() == TypeBox::CLOSED) {
            _flagMap.set(coord, TypeBox::OPENED);
        }
    }

    void Flag::setNoBombToFlagSaveBox(Game::Coord coord) noexcept {
        if (_flagMap.get(coord).type() == TypeBox::FLAG) {
            _flagMap.set(coord, TypeBox::NO_BOMB);
        }
    }
}