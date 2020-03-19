#ifndef ADRJNK_MINESWEEPERBOARD_HPP
#define ADRJNK_MINESWEEPERBOARD_HPP
#include "Field.hpp"

enum GameMode  { DEBUG, EASY=10, NORMAL=20, HARD=30 };
enum GameState { RUNNING, FINISHED_WIN, FINISHED_LOSS };

class MinesweeperBoard {
public:
    MinesweeperBoard(int widght, int height,GameMode mode);
    void debug_display() const;
    int getBoardWidth() const;
    int getBoardHeight() const;
    int getMineCount() const;
    int countMines(int x, int y) const;
    bool hasFlag(int x, int y) const;
    void revealField(int x, int y);
    bool isRevealed(int x, int y) const;
    GameState getGameState() const;
    char getFieldInfo(int x, int y) const;
    ~MinesweeperBoard();

private:
    Field board_[100][100];
    GameMode mode_;
    GameState state_;
    int width_;
    void toggleFlag(int x, int y);
    int height_;
};
#endif //ADRJNK_MINESWEEPERBOARD_HPP
