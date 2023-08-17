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
    // ���� �����͸� �߰��ϴ� �Լ�

    std::string name, line, tier;
    int score;
    
    std::cout << "�����͸� �߰��մϴ�\n";
    std::cout << "�̸�: ";
    std::getline(std::cin, name);

    do
    {
        // ��ȿ�� �Է����� Ȯ���ϴ� ����

        bool IsValidLine = false;

        std::cout << "����(TOP, JUG, MID, AD, SUP): ";
        std::getline(std::cin, line);
        std::transform(line.begin(), line.end(), line.begin(), std::toupper); // ���ڿ� �빮�ڷ� ��ȯ
        
        for (const auto& pair : Config::line)
        {
            if (pair.second == line || pair.first == line)
            {
                IsValidLine = true;
                break;
            }
        }

        if (!IsValidLine)
            std::cout << "�ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է��� �ּ���\n";

        else
            break;
    } while (true);

    do
    {
        // ��ȿ�� �Է����� Ȯ���ϴ� ����

        bool IsValidTier = false;

        std::cout << "Ƽ��(M, E1, P2, U...): ";
        std::getline(std::cin, tier);
        std::transform(tier.begin(), tier.end(), tier.begin(), std::toupper); // ���ڿ� �빮�� ��ȯ

        for (const auto& str : Config::tier)
        {
            if (str == tier)
            {
                IsValidTier = true;
                break;
            }
        }

        if (!IsValidTier)
            std::cout << "�ùٸ��� ���� �Է��Դϴ�. �ٽ� �Է��� �ּ���\n";

        else
            break;

    } while (true);

    score = Config::tier_score[tier]; // Ƽ� �´� ���� �Ҵ�

    _UR.AddUser(name, tier, line, score); // MySQL CRUD
}