#ifndef DB_H
#define DB_H

#include <mysql.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>

class DB
{
	MYSQL _mysql;
    static DB* _instance;
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

    DB();
    ~DB();
    DB(const DB&) = delete; // 복사 생성자 삭제 
    DB& operator = (const DB&) = delete; // 복사 대입 연산자 삭제

public:
    static DB& GetInstance(); 
	bool IsQuerryError(std::string query);
	void TranslateToMySQL();
    void ShowDB();
};

#endif