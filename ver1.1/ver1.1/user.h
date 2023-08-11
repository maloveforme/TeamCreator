#ifndef USER_H
#define USER_H

#include "DB.h"
#include <vector>
#include <string>
#include <map>

struct User
{
    std::string _name, _tier, _line;
    int score;
};

class Player
{
    DB* _db;
    std::vector<User> _player;
    std::map<std::string, int> _tier_board =
    {
        {"M", 43},
        {"D1", 39},
        {"D2", 35},
        {"D3", 31},
        {"D4", 29},
        {"P1", 27},
        {"P2", 25},
        {"P3", 23},
        {"P4", 21},
        {"G1", 19},
        {"G2", 17},
        {"G3", 16},
        {"G4", 14},
        {"S1", 12},
        {"S2", 10},
        {"S3", 9},
        {"S2", 9},
        {"S1", 9},
        {"B4", 9},
        {"B3", 9},
        {"B2", 9},
        {"B1", 9}
    };

public:
    Player();
    ~Player();
    void AddPlayer(const std::string& name);

};

#endif // !USER_H