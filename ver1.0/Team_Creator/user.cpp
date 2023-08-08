#include "user.h"

User::User()
{
	// MySQL 초기화
	mysql_init(&mysql);
	mysql_set_character_set(&mysql, "utf8");

	if (!mysql_real_connect(&mysql, "localhost", "root", "malove8466", "TeamCreator", 3306, NULL, 0))
		std::cerr << "MySQL과의 연동에 실패하였습니다: " << mysql_error(&mysql) << "\n";
}

bool User::IsQuerryError(std::string query)
{
	if (mysql_query(&mysql, query.c_str()) != 0)
	{
		std::cerr << "쿼리 오류: " << mysql_error(&mysql) << "\n";
		return true;
	}

	return false;
}

void User::AddData()
{
	// 데이터를 추가하는 함수
	std::string line, tier, name;

	std::cout << "이름: ";
	getline(std::cin, name);

	std::cout << "라인: ";
	std::cin >> line;

	auto it = line_translations.find(line);

	if (it != line_translations.end())
		line = it->second;

	std::cout << "티어(예:M, D1, P1): ";
	std::cin >> tier;

	int score = tier_score[tier];

	// 해당 플레이어가 이미 존재하는 데이터인지 체크하는 쿼리
	// 이때 존재의 기준은 동일한 "이름"과 "라인"이 있는지
	std::string check_query
		= "SELECT * FROM users WHERE name = '" + name + "' AND line = '" + line + "';";

	if (IsQuerryError(check_query))
		return;

	MYSQL_RES* result = mysql_store_result(&mysql);

	if (result && mysql_num_rows(result) > 0)
	{
		std::cout << "데이터가 이미 존재합니다.\n";
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

		std::cout << "데이터가 추가되었습니다\n";
	}

	mysql_free_result(result);
	
}

void User::ShowUser()
{
	// 현재 DB 내의 저장된 유저들의 정보를 출력하는 함수
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
	// 밸런스 짜기 시작
	// 현재 DB 내의 10명의 인원이 있는지 확인
	std::string count_query 
		= "SELECT COUNT(DISTINCT name) FROM users";

	if (IsQuerryError(count_query))
		return;

	MYSQL_RES* result = mysql_store_result(&mysql);
	MYSQL_ROW row = mysql_fetch_row(result);
		
	if (std::stoi(row[0]) < 10)
		std::cout << "인원이 10명이 되지 않으므로 밸런스를 짤 수 없습니다\n";

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
				ShowBoard();
				break;
			case 2:
				SetUser();
				break;
			case 3:
				ChangeUser();
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

void User::ShowBoard()
{
	// 현재 5:5 현황판을 A팀과 B팀을 기준으로 출력
	// 이때 유저의 이름과 티어를 보여줌
	// 그리고 A팀과 B팀 인원 중 동일 포지션에서 누가 더 뛰어난지 부등호를 이용해 출력
	std::cout << "A팀 " << std::setw(4) << total_score[0] << "     B팀 " << std::setw(4) << total_score[1] << "\n";

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
	// 해당 플레이어를 5:5 보드에 삽입
	std::string name, line;
	char team;

	std::cout << "추가하고자 하는 이름과 라인, 팀을 공백으로 구분하여 입력하세요(ex) 최석원 TOP A: ";
	std::cin >> name >> line >> team;

	std::string query
		= "SELECT tier, score FROM users WHERE name = '" + name + "' AND line = '" + line + "';";

	if (IsQuerryError(query))
		std::cout << "일치하는 정보가 없습니다\n";

	else
	{
		MYSQL_RES* result = mysql_store_result(&mysql);

		if (result)
		{
			MYSQL_ROW row = mysql_fetch_row(result);
			board[enum_line[line]][enum_team[team]] = { name, row[0] };
			total_score[enum_team[team]] += std::stoi(row[1]);

			mysql_free_result(result);
			std::cout << "인원 추가에 성공했습니다\n";
			ShowBoard();
		}

		else
			std::cout << "일치하는 정보가 없습니다\n";
	}
}

std::pair<int, int> User::FindUserIndex(const std::string& name)
{
	// name이라는 플레이어가 5:5 보드판의 어디에 위치해 있는지 출력
	// 만약 찾지 못했다면 -1, -1 리턴

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
	std::cout << "선수끼리 라인을 바꿀 것이면 1번을, 인원 자체를 바꾸실 것이면 2번을 입력하세요: ";
	std::cin >> choice;

	if (choice == 1)
	{
		// 같은 보드판 내의 선수 교체 
		// 추후에 선수를 교체하며 바뀐 라인의 정보도 추가할 예정
		std::string name1, name2;
		std::cout << "바꿀 선수의 이름을 공백으로 구분하여 입력해주세요: ";
		std::cin >> name1 >> name2;

		std::pair<int, int> name1_index = FindUserIndex(name1);
		std::pair<int, int> name2_index = FindUserIndex(name2);

		std::swap(board[name1_index.first][name1_index.second],
			board[name2_index.first][name2_index.second]);

		ShowBoard();
	}

	else if (choice == 2)
	{
		// 5:5 보드판에서 인원이 빠지고, 그 자리를 DB 내의 새로운 유저가 들어감
		// 이때 DB에 없는 사람이면 관련 정보를 추가하는 것도 추가될 예정
		std::string name1, name2, query;
		std::cout << "빠지는 선수의 이름을 입력해주세요: ";
		std::cin >> name1;

		std::cout << "추가되는 선수의 이름을 입력해주세요: ";
		std::cin >> name2;

		std::map<int, std::string> reverse_line_map;
		std::pair<int, int> name1_index = FindUserIndex(name1);

		if (name1_index.first == -1)
		{
			std::cout << "해당 이름을 가진 선수가 없습니다. 다시 입력해주세요." << std::endl;
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
						std::cout << "유효하지 않은 데이터입니다.\n";

				}

				else
					std::cout << "DB에 데이터가 없습니다.\n";

				mysql_free_result(result);
			}
			else
				std::cout << "DB에서 결과를 가져오는 데 실패했습니다.\n";
		}
	}
}

User::~User()
{
	mysql_close(&mysql);
}

