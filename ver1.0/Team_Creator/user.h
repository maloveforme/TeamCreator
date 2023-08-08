#ifndef USER_H
#define USER_H

#include <string>
#include <mysql.h>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>

class User
{
    MYSQL mysql;
    std::pair<std::string, std::string> board[5][2];
    std::vector<std::string> player;
    int total_score[2];

    std::map<std::string, std::string> line_translations =
    {
        {"Å¾", "TOP"},
        {"¹Ìµå", "MID"},
        {"Á¤±Û", "JUG"},
        {"¿øµô", "AD"},
        {"¼­Æý", "SUP"},
        {"TOP", "TOP"},
        {"MID", "MID"},
        {"JUG", "JUG"},
        {"AD", "AD"},
        {"SUP", "SUP"}
    };

    std::map<std::string, int> tier_score =
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

    std::map<std::string, int> enum_line =
    {
        {"TOP", 0},
        {"JUG", 1},
        {"MID", 2},
        {"AD", 3},
        {"SUP", 4}
    };

    std::map<char, int> enum_team =
    {
        {'A', 0},
        {'B', 1}
    };

public:
    User();
    ~User();

    void AddData();
    void Run();
    void ShowUser();
    bool IsQuerryError(std::string query);
    void ShowBoard();
    void SetUser();
    void ChangeUser();
    std::pair<int, int> FindUserIndex(const std::string& name);
};

#endif // USER_H