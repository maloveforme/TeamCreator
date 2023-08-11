#include "play.h"

void Play::Start()
{
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
            std::cout << "�߸��� �Է��Դϴ�\n";
            break;
        }
    }
}

void Play::ShowUser()
{
    std::cout << "���� �����ͺ��̽� ���� �����ϴ� ������� �̸��� ������ ����մϴ�\n";
    std::cout << "�̸�\t����\tƼ��\t����\n";
}

void Play::Run()
{
    std::string query
        = "SELECT COUNT(DISTINCT name) FROM users";

    MYSQL_RES* result = _db->FetchRows(query);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (std::stoi(row[0]) < 10)
    {
        std::cout << "�ο��� 10���� ���� �����Ƿ� �뷱���� © �� �����ϴ�.\n";
        std::cout << "�����͸� �߰����ּ���\n";
    }

    else
    {
        int choice;

        do
        {
            std::cout << "���� ���� ����(1), �ο� ����(2), �ο� ����(3), ����(4): ";
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
                std::cout << "���α׷��� �����մϴ�.\n";
                break;
            default:
                std::cout << "�߸��� �Է��Դϴ�. �ٽ� �õ��� �ּ���.\n";
            }
        } while (choice != 4);
    }
}

void Play::AddData()
{

}