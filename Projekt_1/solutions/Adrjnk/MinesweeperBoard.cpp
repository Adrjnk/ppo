#include <iostream>
#include <cstdlib>
#include <ctime>
#include "MinesweeperBoard.hpp"

MinesweeperBoard::MinesweeperBoard(int width, int height,GameMode mode):width(width),height(height),mode(mode)
{
    srand( time( NULL ) );
    
    if(mode!=DEBUG) {

        int mines = (this->width * this->height * this->mode) / 100;
        while (mines > 0) {
            int x = rand() % this->getBoardWidth();
            int y = rand() % this->getBoardHeight();
            if (!this->board[x][y].hasMine)
            {
                this->board[x][y].hasMine = 1;
                mines -= 1;
            }

        }
    }
    if(mode==DEBUG) {

        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                if (i == j)
                    board[i][j].hasMine = 1;
                if (j == 0)
                    board[i][j].hasMine = 1;
                if (i == 0 && j % 2 == 0)
                    board[i][j].hasMine = 1;
            }
        }
    }
}
void MinesweeperBoard::debug_display() const {

    for(int j=0; j<this->height; j++)
    {
        for(int i=0; i<this->width; i++)
        {
            std::cout<<"[";
            if(board[i][j].hasMine==0)
                std::cout<<".";
            else if(board[i][j].hasMine==1)
                std::cout<<"M";
            if(board[i][j].isRevealed==0)
                std::cout<<".";
            else if(board[i][j].isRevealed==1)
                std::cout<<"o";
            if(board[i][j].hasFlag==0)
                std::cout<<".";
            else if(board[i][j].hasFlag==1)
                std::cout<<"f";
            std::cout<<"]";
        }
        std::cout<<std::endl;
    }
}
int MinesweeperBoard::getBoardWidth() const {
    return width;
}
int MinesweeperBoard::getBoardHeight() const {
    return height;
}
int MinesweeperBoard::getMineCount() const {
    int bombs=0;

        for(int j=0; j<height; j++)
        {
            for(int i=0; i<width; i++)
            {
               if(board[i][j].hasMine == true)
                   bombs++;
            }
        }
        return bombs;
}
int MinesweeperBoard::countMines(int x, int y) const {
    int bombs=0;
    if(x>width || x<0 || y>height || y<0)
        return -1;
    if(board[x][y].isRevealed==0)
        return -1;
    for(int j=0; j<=2; j++)
    {
        for(int i=0; i<=2; i++)
        {
            if(board[x+i-1][y+j-1].hasMine==1 && (x+i-1)>=0 && (y+j-1)>=0)
                bombs++;
        }
    }
    return bombs;
}
bool MinesweeperBoard::hasFlag(int x, int y) const{
    if(this->areCordsValid(x,y)==true && board[x][y].hasFlag==1 && board[x][y].isRevealed==0)
        return true;
    else
        return false;
}
void MinesweeperBoard::toggleFlag(int x, int y) {

    if(board[x][y].isRevealed==1 || this->areCordsValid(x,y)==true || state==FINISHED_WIN || state==FINISHED_LOSS)
    {
        return;
    }
    else {
        if (board[x][y].isRevealed == 0 && board[x][y].hasFlag == 0)
            board[x][y].hasFlag=1;
        if (board[x][y].isRevealed == 0 && board[x][y].hasFlag == 1)
            board[x][y].hasFlag=0;
    }
}
void MinesweeperBoard::revealField(int x, int y){
    srand( time( NULL ) );
    int revaled=0;
    if(board[x][y].isRevealed==1 || board[x][y].hasFlag==1 ||  this->areCordsValid(x,y)==true || state==FINISHED_WIN || state==FINISHED_LOSS){
        return;
    }
    else{
        if(board[x][y].isRevealed==0 && board[x][y].hasMine==0)
        {
            board[x][y].isRevealed=1;
        }
        else
            {
        for(int j=0; j<height; j++)
        {
            for(int i=0; i<width; i++)
            {
                if(board[i][j].isRevealed==1)
                    revaled++;
            }
        }
        if(revaled==0 && board[x][y].hasMine==1 && mode!=DEBUG) {
            int a = rand() % this->getBoardWidth();
            int b = rand() % this->getBoardHeight();
            board[x][y].hasMine = 0;
            board[a][b].hasMine = 1;
            }
        else if(revaled>0 && board[x][y].hasMine==1 && mode!=DEBUG) {
            board[x][y].isRevealed = 1;
            state = FINISHED_LOSS;
            }
        }
    }
}
/*if (this->board[x][y].isRevealed && this->areCordsValid(x, y) && this->state == GameState::Running) {
this->board[x][y].hasFlag = !this->board[x][y].hasFlag;
}
*/
bool MinesweeperBoard::isRevealed(int x, int y) const{
    if(board[x][y].isRevealed)
        return true;
    else
        return false;
}
GameState MinesweeperBoard::getGameState() const{
    int flagged;
    for(int j=0; j<height; j++)
    {
        for(int i=0; i<width; i++)
        {
            if(board[i][j].hasMine==1 && board[i][j].isRevealed==1)
                return FINISHED_LOSS;
            if(board[i][j].hasMine==board[i][j].hasFlag)
                flagged++;
        }
    }
    if(((this->width * this->height * this->mode)/100)==flagged)
        return FINISHED_WIN;
    else
        return RUNNING;
}
char MinesweeperBoard::getFieldInfo(int x, int y) const{
    if(x>0 || x<width || y>height || y<0)
    return 'x';
    if(board[x][y].isRevealed==1 && board[x][y].hasFlag==0)
        return 'F';
    if(board[x][y].isRevealed==1 && board[x][y].hasFlag==1)
        return '_';
    if(board[x][y].isRevealed==1 && board[x][y].hasMine==1)
        return 'x';
    if(board[x][y].isRevealed==1 && countMines(x,y)==0)
        return ' ';
    if(board[x][y].isRevealed==1 && countMines(x,y)>0)
        return countMines(x,y)+'0';
    else
        return '0';
}
bool MinesweeperBoard::areCordsValid(int x, int y) const{
    if(x>width || x<0 || y>height || y<0)
        return false;
    else
        return true;

}
MinesweeperBoard::~MinesweeperBoard(){}