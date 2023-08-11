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
    DB(const DB&) = delete; // ���� ������ ���� 
    DB& operator = (const DB&) = delete; // ���� ���� ������ ����

public:
    static DB* GetInstance();
    bool ExecuteQuery(const std::string& query);
    MYSQL_RES* FetchRows(const std::string& query);
};

class UserRepository
{
    DB* _db;

public:
    UserRepository();
    void PrintUser();
};

#endif
