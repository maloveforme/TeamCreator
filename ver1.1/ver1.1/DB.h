#ifndef DB_H
#define DB_H

#include <mysql.h>
#include <iostream>
#include <string>

class DB
{
    MYSQL _mysql;
    static DB* _instance;

    DB();
    ~DB();
    DB(const DB&) = delete; // 복사 생성자 삭제 
    DB& operator = (const DB&) = delete; // 복사 대입 연산자 삭제

public:
    static DB* GetInstance();
    bool ExecuteQuery(const std::string& query);
    MYSQL_RES* FetchRows(const std::string& query);
    std::string GetError() const;
};

class UserRepository
{
    DB* _db;

public:
    UserRepository();
    void PrintUser();
    void AddUser(std::string name, std::string tier, std::string line, int score);
};



#endif
