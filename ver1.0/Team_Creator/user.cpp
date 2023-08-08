#include "user.h"

User::User()
{
	// MySQL �ʱ�ȭ
	mysql_init(&mysql);
	mysql_set_character_set(&mysql, "utf8");

	if (!mysql_real_connect(&mysql, "localhost", "root", "malove8466", "TeamCreator", 3306, NULL, 0))
		std::cerr << "MySQL���� ������ �����Ͽ����ϴ�: " << mysql_error(&mysql) << "\n";
}

bool User::IsQuerryError(std::string query)
{
	if (mysql_query(&mysql, query.c_str()) != 0)
	{
		std::cerr << "���� ����: " << mysql_error(&mysql) << "\n";
		return true;
	}

	return false;
}

void User::AddData()
{
	// �����͸� �߰��ϴ� �Լ�
	std::string line, tier, name;

	std::cout << "�̸�: ";
	getline(std::cin, name);

	std::cout << "����: ";
	std::cin >> line;

	auto it = line_translations.find(line);

	if (it != line_translations.end())
		line = it->second;

	std::cout << "Ƽ��(��:M, D1, P1): ";
	std::cin >> tier;

	int score = tier_score[tier];

	// �ش� �÷��̾ �̹� �����ϴ� ���������� üũ�ϴ� ����
	// �̶� ������ ������ ������ "�̸�"�� "����"�� �ִ���
	std::string check_query
		= "SELECT * FROM users WHERE name = '" + name + "' AND line = '" + line + "';";

	if (IsQuerryError(check_query))
		return;

	MYSQL_RES* result = mysql_store_result(&mysql);

	if (result && mysql_num_rows(result) > 0)
	{
		std::cout << "�����Ͱ� �̹� �����մϴ�.\n";
		mysql_free_result(result);
		return;
	}

	else
	{
		std::string insert_query
			= "INSERT INTO users (name, line, tier, score) VALUES ('" + name + "', '"
			+ line + "', '" + tier + "', " + std::to_string(score) + ");";

		if (IsQuerryError(check_query))
			return;

		std::cout << "�����Ͱ� �߰��Ǿ����ϴ�\n";
	}

	mysql_free_result(result);
	
}

void User::ShowUser()
{
	// ���� DB ���� ����� �������� ������ ����ϴ� �Լ�
	std::string show_query 
		= "SELECT name, line, tier, score FROM users ORDER BY name;";

	if (IsQuerryError(show_query))
		return;

	else
	{
		MYSQL_RES* result = mysql_store_result(&mysql);

		if (result)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(result)))
			{
				for (int i = 0; i < mysql_num_fields(result); i++)
					std::cout << row[i] << "\t";
				std::cout << "\n";
			}
			mysql_free_result(result);

			ShowBoard();
		}
	}
}

void User::Run()
{
	// �뷱�� ¥�� ����
	// ���� DB ���� 10���� �ο��� �ִ��� Ȯ��
	std::string count_query 
		= "SELECT COUNT(DISTINCT name) FROM users";

	if (IsQuerryError(count_query))
		return;

	MYSQL_RES* result = mysql_store_result(&mysql);
	MYSQL_ROW row = mysql_fetch_row(result);
		
	if (std::stoi(row[0]) < 10)
		std::cout << "�ο��� 10���� ���� �����Ƿ� �뷱���� © �� �����ϴ�\n";

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
				ShowBoard();
				break;
			case 2:
				SetUser();
				break;
			case 3:
				ChangeUser();
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

void User::ShowBoard()
{
	// ���� 5:5 ��Ȳ���� A���� B���� �������� ���
	// �̶� ������ �̸��� Ƽ� ������
	// �׸��� A���� B�� �ο� �� ���� �����ǿ��� ���� �� �پ�� �ε�ȣ�� �̿��� ���
	std::cout << "A�� " << std::setw(4) << total_score[0] << "     B�� " << std::setw(4) << total_score[1] << "\n";

	for (int i = 0; i < 5; ++i)
	{
		char ch = ' ';

		if (!board[i][0].first.empty() && !board[i][1].first.empty())
		{
			int a_team_score = tier_score[board[i][0].second];
			int b_team_score = tier_score[board[i][1].second];

			if (a_team_score > b_team_score)
				ch = '>';

			else if (a_team_score == b_team_score)
				ch = '=';

			else if (a_team_score < b_team_score)
				ch = '<';
		}

		std::cout << std::setw(4) << std::left << board[i][0].first << ' ' << std::setw(2) << std::left << board[i][0].second;
		std::cout << " " << ch << "  ";
		std::cout << "  " << std::setw(4) << std::left << board[i][1].first << ' ' << std::setw(2) << std::left << board[i][1].second << "\n";
	}
}

void User::SetUser()
{
	// �ش� �÷��̾ 5:5 ���忡 ����
	std::string name, line;
	char team;

	std::cout << "�߰��ϰ��� �ϴ� �̸��� ����, ���� �������� �����Ͽ� �Է��ϼ���(ex) �ּ��� TOP A: ";
	std::cin >> name >> line >> team;

	std::string query
		= "SELECT tier, score FROM users WHERE name = '" + name + "' AND line = '" + line + "';";

	if (IsQuerryError(query))
		std::cout << "��ġ�ϴ� ������ �����ϴ�\n";

	else
	{
		MYSQL_RES* result = mysql_store_result(&mysql);

		if (result)
		{
			MYSQL_ROW row = mysql_fetch_row(result);
			board[enum_line[line]][enum_team[team]] = { name, row[0] };
			total_score[enum_team[team]] += std::stoi(row[1]);

			mysql_free_result(result);
			std::cout << "�ο� �߰��� �����߽��ϴ�\n";
			ShowBoard();
		}

		else
			std::cout << "��ġ�ϴ� ������ �����ϴ�\n";
	}
}

std::pair<int, int> User::FindUserIndex(const std::string& name)
{
	// name�̶�� �÷��̾ 5:5 �������� ��� ��ġ�� �ִ��� ���
	// ���� ã�� ���ߴٸ� -1, -1 ����

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (board[i][j].first == name)
				return std::make_pair(i, j);
		}
	}

	return std::make_pair(-1, -1);
}

void User::ChangeUser()
{
	int choice;
	std::cout << "�������� ������ �ٲ� ���̸� 1����, �ο� ��ü�� �ٲٽ� ���̸� 2���� �Է��ϼ���: ";
	std::cin >> choice;

	if (choice == 1)
	{
		// ���� ������ ���� ���� ��ü 
		// ���Ŀ� ������ ��ü�ϸ� �ٲ� ������ ������ �߰��� ����
		std::string name1, name2;
		std::cout << "�ٲ� ������ �̸��� �������� �����Ͽ� �Է����ּ���: ";
		std::cin >> name1 >> name2;

		std::pair<int, int> name1_index = FindUserIndex(name1);
		std::pair<int, int> name2_index = FindUserIndex(name2);

		std::swap(board[name1_index.first][name1_index.second],
			board[name2_index.first][name2_index.second]);

		ShowBoard();
	}

	else if (choice == 2)
	{
		// 5:5 �����ǿ��� �ο��� ������, �� �ڸ��� DB ���� ���ο� ������ ��
		// �̶� DB�� ���� ����̸� ���� ������ �߰��ϴ� �͵� �߰��� ����
		std::string name1, name2, query;
		std::cout << "������ ������ �̸��� �Է����ּ���: ";
		std::cin >> name1;

		std::cout << "�߰��Ǵ� ������ �̸��� �Է����ּ���: ";
		std::cin >> name2;

		std::map<int, std::string> reverse_line_map;
		std::pair<int, int> name1_index = FindUserIndex(name1);

		if (name1_index.first == -1)
		{
			std::cout << "�ش� �̸��� ���� ������ �����ϴ�. �ٽ� �Է����ּ���." << std::endl;
			return;
		}

		for (const auto& pair : enum_line)
			reverse_line_map.insert({ pair.second, pair.first });

		std::string line = reverse_line_map[name1_index.first];

		query =
			"SELECT tier, score FROM users WHERE name = '" + name2 + "' AND line = '" + line + "';";

		if (!IsQuerryError(query))
		{
			mysql_query(&mysql, query.c_str());

			MYSQL_RES* result = mysql_store_result(&mysql);

			if (result != NULL)
			{
				MYSQL_ROW row = mysql_fetch_row(result);

				if (row != NULL)
				{
					std::string tier = row[0];

					if (tier_score.find(tier) != tier_score.end())
					{
						int old_score = tier_score[board[name1_index.first][name1_index.second].second];
						int new_score = tier_score[tier];
						total_score[name1_index.second] = total_score[name1_index.second] - old_score + new_score;

						board[name1_index.first][name1_index.second] = { name2, tier };
					}

					else
						std::cout << "��ȿ���� ���� �������Դϴ�.\n";

				}

				else
					std::cout << "DB�� �����Ͱ� �����ϴ�.\n";

				mysql_free_result(result);
			}
			else
				std::cout << "DB���� ����� �������� �� �����߽��ϴ�.\n";
		}
	}
}

User::~User()
{
	mysql_close(&mysql);
}

