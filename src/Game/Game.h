#pragma once

#include <memory>

namespace Game {
    class Flag;
    class Bomb;
    class Box;
    struct Coord;

    enum class GameState {
        PLAYED,
        BOMBED,
        WINNER
    };

    class Game {
    public:
        Game() = delete;
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

        Game(int cols, int rows, int bombs) noexcept;

        void render() const noexcept;
        void start() noexcept;
        bool gameOver() noexcept;
        void pressRightButton(const Coord coord) noexcept;
        void pressLeftButton(const Coord coord) noexcept;

        Box getBox(const Coord coord);
    private:
        void openBox(const Coord coord) noexcept;
        void openBoxesAround(const Coord coord) noexcept;
        void openBombs(const Coord bombed) noexcept;
        void checkWinner() noexcept;

        void setOpenedToClosedBoxesAround(const Coord coord);

        GameState _state;
        std::shared_ptr<Flag> _flag;
        std::shared_ptr<Bomb> _bomb;
    };
}