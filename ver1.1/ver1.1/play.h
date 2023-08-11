#ifndef PLAY_H
#define PLAY_H

#include "user.h"
#include "DB.h"
#include "board.h"

class Play
{
    UserRepository _UR;
    Board& _board;
    DB* _db;

public:
    Play() : _board(Board::GetInstance()), _db(DB::GetInstance()) {}
    void Start();
    void ShowUser();
    void Run();
    void AddData();
};

#endif // !PLAY_H