#include "main.h"

int main()
{
    Board& board = Board::GetInstance();
    DB& db = DB::GetInstance();
    std::string name;
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
            std::cout << "현재 데이터베이스 내에 존재하는 사용자의 이름과 정보를 출력합니다\n";
            std::cout << "이름\t라인\t티어\t점수\n";
            db.ShowDB();
            break;

        case 2:
            
            break;

        case 3:
            
            break;

        case 4:
            return 0;

        default:
            std::cout << "잘못된 입력입니다\n";
            break;
        }
    }
}