#include "Game.h"

#include "../ResourceManager/ResourceManager.h"
#include "Flag.h"
#include "Bomb.h"
#include "Ranges.h"

namespace Game {
    Game::Game(int cols, int rows, int bombs) noexcept :
        _state(GameState::BOMBED) {
        Ranges::instance().setSize({cols, rows});
        _bomb = std::make_shared<Bomb>(bombs);
        _flag = std::make_shared<Flag>();
    }

    void Game::render() const noexcept {
        _bomb->render();
        _flag->render();
    }

    void Game::start() noexcept {
        _bomb->start();
        _flag->start();
        _state = GameState::PLAYED;
    }

    bool Game::gameOver() noexcept {
        if (_state == GameState::PLAYED) {
            return false;
        }
        start();
        return true;
    }

    void Game::pressRightButton(const Coord coord) noexcept {
        if (gameOver()) {
            return;
        }
        _flag->toggleFlaggedToBox(coord);
        checkWinner();
    }

    void Game::pressLeftButton(const Coord coord) noexcept {
        if (gameOver()) {
            return;
        }
        openBox(coord);
        checkWinner();
    }

    Box Game::getBox(const Coord coord) {
        if (_flag->get(coord).type() == TypeBox::OPENED) {
            return _bomb->get(coord);
        }
        return _flag->get(coord);
    }

    void Game::openBox(const Coord coord) noexcept {
        switch (_flag->get(coord).type()) {
            case TypeBox::OPENED:
                setOpenedToClosedBoxesAround(coord);
                return;
            case TypeBox::FLAG:
                return;
            case TypeBox::CLOSED:
                switch (_bomb->get(coord).type()) {
                    case TypeBox::ZERO:
                        openBoxesAround(coord);
                        return;
                    case TypeBox::BOMB:
                        openBombs(coord);
                        return;
                    default:
                        _flag->setOpenedToBox(coord);
                        return;
                }
            default:
                return;
        }
    }

    void Game::openBoxesAround(const Coord coord) noexcept {
        _flag->setOpenedToBox(coord);
        for (const auto& currCoord : Ranges::instance().getCoordsAround(coord)) {
            openBox(currCoord);
        }
    }

    void Game::openBombs(const Coord bombed) noexcept {
       _state = GameState::BOMBED;
       _flag->setBombedToBox(bombed);
       for (const auto& coord : Ranges::instance().getAllCoord()) {
           if (_bomb->get(coord).type() == TypeBox::BOMB) {
               _flag->setOpenedToClosedBox(coord);
           } else {
               _flag->setNoBombToFlagSaveBox(coord);
           }
       }
    }

    void Game::checkWinner() noexcept {
        if (_state == GameState::PLAYED and
            _flag->getCountClosedBox() == _bomb->getTotalBombs() and
            _flag->getCountFlaggedBox() == _bomb->getTotalBombs()) {
            _state = GameState::WINNER;
        }
    }

    void Game::setOpenedToClosedBoxesAround(Coord coord) {
		if (_bomb->get(coord).type() != TypeBox::BOMB and
            _flag->getCountFlaggedBoxesAround(coord) == _bomb->get(coord).getNumber()) {
            for (Coord around: Ranges::instance().getCoordsAround(coord)) {
                if (_flag->get(around).type() == TypeBox::CLOSED) {
                    openBox(around);
                }
            }
		}
	}
}