#include <iostream>
#include <cstdlib>
#include <ctime>
#include "MinesweeperBoard.hpp"

MinesweeperBoard::MinesweeperBoard(int widght, int height,GameMode mode):width_(widght),height_(height),mode_(mode)
{
    srand( time( NULL ) );

    for(int j=0; j<height; j++)
    {
        for(int i=0; i<widght; i++)
        {
            board_[i][j].hasMine=0;
            board_[i][j].hasFlag=0;
            board_[i][j].isRevealed=0;
        }
    }

    if(mode!=DEBUG) {

        int mines = (this->width_ * this->height_ * this->mode_) / 100;
        while (mines > 0) {
            int x = rand() % this->getBoardWidth();
            int y = rand() % this->getBoardHeight();
            if (!this->board_[x][y].hasMine)
            {
                this->board_[x][y].hasMine = 1;
                mines -= 1;
            }

        }
    }
    if(mode==DEBUG) {

        for (int j = 0; j < height; j++) {
            for (int i = 0; i < widght; i++) {
                if (i == j)
                    board_[i][j].hasMine = 1;
                if (j == 0)
                    board_[i][j].hasMine = 1;
                if (i == 0 && j % 2 == 0)
                    board_[i][j].hasMine = 1;
            }
        }
    }
}
void MinesweeperBoard::debug_display() const {

    for(int j=0; j<this->height_; j++)
    {
        for(int i=0; i<this->width_; i++)
        {
            std::cout<<"[";
            if(board_[i][j].hasMine==0)
                std::cout<<".";
            else if(board_[i][j].hasMine==1)
                std::cout<<"M";
            if(board_[i][j].isRevealed==0)
                std::cout<<".";
            else if(board_[i][j].isRevealed==1)
                std::cout<<"o";
            if(board_[i][j].hasFlag==0)
                std::cout<<".";
            else if(board_[i][j].hasFlag==1)
                std::cout<<"f";
            std::cout<<"]";
        }
        std::cout<<std::endl;
    }
}
int MinesweeperBoard::getBoardWidth() const {
    return width_;
}
int MinesweeperBoard::getBoardHeight() const {
    return height_;
}
int MinesweeperBoard::getMineCount() const {
    int bombs=0;

        for(int j=0; j<height_; j++)
        {
            for(int i=0; i<width_; i++)
            {
               if(board_[i][j].hasMine==1)
                   bombs++;
            }
        }
        return bombs;
}
int MinesweeperBoard::countMines(int x, int y) const {
    int bombs=0;
    if(x>width_ || x<0 || y>height_ || y<0)
        return -1;
    if(board_[x][y].isRevealed==0)
        return -1;
    for(int j=0; j<=2; j++)
    {
        for(int i=0; i<=2; i++)
        {
            if(board_[x+i-1][y+j-1].hasMine==1 && (x+i-1)>=0 && (y+j-1)>=0)
                bombs++;
        }
    }
    return bombs;
}
bool MinesweeperBoard::hasFlag(int x, int y) const{
    if(board_[x][y].hasFlag==1)
        return 1;
    if(x>width_ || x<0 || y>height_ || y<0 || board_[x][y].hasFlag==0 || board_[x][y].isRevealed==1)
        return 0;
    else
        return 0;
}
void MinesweeperBoard::toggleFlag(int x, int y) {

    if(board_[x][y].isRevealed==1 || x>width_ || x<0 || y>height_ || y<0 || state_==FINISHED_WIN || state_==FINISHED_LOSS)
    {
        return;
    }
    else {
        if (board_[x][y].isRevealed == 0 && board_[x][y].hasFlag == 0)
            board_[x][y].hasFlag == 1;
        if (board_[x][y].isRevealed == 0 && board_[x][y].hasFlag == 1)
            board_[x][y].hasFlag == 0;
    }
}
void MinesweeperBoard::revealField(int x, int y){
    srand( time( NULL ) );
    int revaled=0;
    if(board_[x][y].isRevealed==1 || board_[x][y].hasFlag==1 ||  x>width_ || x<0 || y>height_ || y<0 || state_==FINISHED_WIN || state_==FINISHED_LOSS){
        return;
    }
    else{
        if(board_[x][y].isRevealed==0 && board_[x][y].hasMine==0)
        {
            board_[x][y].isRevealed=1;
        }
        else
            {
        for(int j=0; j<height_; j++)
        {
            for(int i=0; i<width_; i++)
            {
                if(board_[i][j].isRevealed==1)
                    revaled++;
            }
        }
        if(revaled==0 && board_[x][y].hasMine==1 && mode_!=DEBUG) {
            int a = rand() % this->getBoardWidth();
            int b = rand() % this->getBoardHeight();
            board_[x][y].hasMine = 0;
            board_[a][b].hasMine = 1;
            }
        else if(revaled>0 && board_[x][y].hasMine==1 && mode_!=DEBUG) {
            board_[x][y].isRevealed = 1;
            state_ = FINISHED_LOSS;
            }
        }
    }
}
bool MinesweeperBoard::isRevealed(int x, int y) const{
    if(board_[x][y].isRevealed==1)
        return 1;
    else
        return 0;
}
GameState MinesweeperBoard::getGameState() const{
    int flagged;
    for(int j=0; j<height_; j++)
    {
        for(int i=0; i<width_; i++)
        {
            if(board_[i][j].hasMine==board_[i][j].isRevealed)
                return FINISHED_LOSS;
            if(board_[i][j].hasMine==board_[i][j].hasFlag)
                flagged++;
        }
    }
    if(((this->width_ * this->height_ * this->mode_)/100)==flagged)
        return FINISHED_WIN;
    else
        return RUNNING;
}
char MinesweeperBoard::getFieldInfo(int x, int y) const{
    if(x>0 || x<width_ || y>height_ || y<0)
    return 'x';
    if(board_[x][y].isRevealed==1 && board_[x][y].hasFlag==0)
        return 'F';
    if(board_[x][y].isRevealed==1 && board_[x][y].hasFlag==1)
        return '_';
    if(board_[x][y].isRevealed==1 && board_[x][y].hasMine==1)
        return 'x';
    if(board_[x][y].isRevealed==1 && countMines(x,y)==0)
        return ' ';
    if(board_[x][y].isRevealed==1 && countMines(x,y)>0)
        return countMines(x,y)+'0';
    else
        return '0';
}
MinesweeperBoard::~MinesweeperBoard(){}