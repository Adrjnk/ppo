#include "MinesweeperBoard.hpp"
#include <iostream>

int main() {
    MinesweeperBoard Board(9,7,DEBUG);
    Board.debug_display();
    std::cout<<"hasFlag:"<<Board.hasFlag(1, 1)<<std::endl;

    std::cout<<"countMines:"<<Board.countMines(1, 0)<<std::endl;

    std::cout<<"GetGameState:"<<Board.getGameState();
    return 0;
}

