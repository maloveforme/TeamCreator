#include "play.h"

void Play::Start()
{
    int choice;

    while (true)
    {
        std::cout << "원하는 기능을 선택하세요\n";
        std::cout << "1.현재 저장된 사용자 보기\t 2.밸런스 짜기\t 3. 데이터 추가\t4.종료: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case 1:
            ShowUser();
            break;

        case 2:
            Run();
            break;

        case 3:
            AddData();
            break;

        case 4:
            return;

        default:
            std::cout << "잘못된 입력입니다\n";
            break;
        }
    }
}

void Play::ShowUser()
{
    std::cout << "현재 데이터베이스 내에 존재하는 사용자의 이름과 정보를 출력합니다\n";
    std::cout << "이름\t라인\t티어\t점수\n";
    _UR.PrintUser();
}

void Play::Run()
{
    std::string query
        = "SELECT COUNT(DISTINCT name) FROM users";

    MYSQL_RES* result = _db->FetchRows(query);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (std::stoi(row[0]) < 10)
    {
        std::cout << "인원이 10명이 되지 않으므로 밸런스를 짤 수 없습니다.\n";
        std::cout << "데이터를 추가해주세요\n";
    }

    else
    {
        int choice;

        do
        {
            std::cout << "현재 보드 보기(1), 인원 배정(2), 인원 수정(3), 종료(4): ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice)
            {
            case 1:
                _board.ShowBoard();
                break;
            case 2:
                _board.SetUser();
                break;
            case 3:
                _board.ChangeUser();
                break;
            case 4:
                std::cout << "프로그램을 종료합니다.\n";
                break;
            default:
                std::cout << "잘못된 입력입니다. 다시 시도해 주세요.\n";
            }

        } while (choice != 4);
    }
}

void Play::AddData()
{
    // 유저 데이터를 추가하는 함수

    std::string name, line, tier;
    int score;
    
    std::cout << "데이터를 추가합니다\n";
    std::cout << "이름: ";
    std::getline(std::cin, name);

    do
    {
        // 유효한 입력인지 확인하는 구문

        bool IsValidLine = false;

        std::cout << "라인(TOP, JUG, MID, AD, SUP): ";
        std::getline(std::cin, line);
        std::transform(line.begin(), line.end(), line.begin(), std::toupper); // 문자열 대문자로 변환
        
        for (const auto& pair : Config::line)
        {
            if (pair.second == line || pair.first == line)
            {
                IsValidLine = true;
                break;
            }
        }

        if (!IsValidLine)
            std::cout << "올바르지 않은 입력입니다. 다시 입력해 주세요\n";

        else
            break;
    } while (true);

    do
    {
        // 유효한 입력인지 확인하는 구문

        bool IsValidTier = false;

        std::cout << "티어(M, E1, P2, U...): ";
        std::getline(std::cin, tier);
        std::transform(tier.begin(), tier.end(), tier.begin(), std::toupper); // 문자열 대문자 변환

        for (const auto& str : Config::tier)
        {
            if (str == tier)
            {
                IsValidTier = true;
                break;
            }
        }

        if (!IsValidTier)
            std::cout << "올바르지 않은 입력입니다. 다시 입력해 주세요\n";

        else
            break;

    } while (true);

    score = Config::tier_score[tier]; // 티어에 맞는 점수 할당

    _UR.AddUser(name, tier, line, score); // MySQL CRUD
}