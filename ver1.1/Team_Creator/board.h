#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <iostream>
#include <iomanip>

class Board
{
    static Board* _instance;
    std::pair<std::string, std::string> _board[5][2];
    int _total_score[2];

    Board();
    ~Board();
    Board(const Board&) = delete;
    Board& operator = (const Board&) = delete;

public:
    static Board& GetInstance();
    void ShowBoard();

};

#endif // BOARD_H