#include "main.h"

int main()
{
    Board& board = Board::GetInstance();
    DB& db = DB::GetInstance();
    std::string name;
    int choice;

    while (true)
    {
        std::cout << "���ϴ� ����� �����ϼ���\n";
        std::cout << "1.���� ����� ����� ����\t 2.�뷱�� ¥��\t 3. ������ �߰�\t4.����: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case 1:
            std::cout << "���� �����ͺ��̽� ���� �����ϴ� ������� �̸��� ������ ����մϴ�\n";
            std::cout << "�̸�\t����\tƼ��\t����\n";
            db.ShowDB();
            break;

        case 2:
            
            break;

        case 3:
            
            break;

        case 4:
            return 0;

        default:
            std::cout << "�߸��� �Է��Դϴ�\n";
            break;
        }
    }
}