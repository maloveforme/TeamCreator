#include "user.h"

Player::Player()
{
	_db = DB::GetInstance();
	_player.reserve(10);
}

Player::~Player()
{

}

void Player::AddPlayer(const std::string& name)
{
	std::string tier, line, query;
	int score;

	std::cout << "Ƽ��: ";
	std::getline(std::cin, tier);
	score = _tier_board[tier];

	std::cout << "����: ";
	std::getline(std::cin, line);

	query
		= "SELECT * FROM users WHERE name = '" + name + "' AND line = '" + line + "';";

	MYSQL_RES* result = _db->FetchRows(query);

	if (!result)
	{
		std::cerr << "�����ͺ��̽����� �����͸� �������� ���߽��ϴ�.\n";
		return;
	}

	if (mysql_num_rows(result) > 0)
	{
		std::cout << "�̹� ������ �̸��� ������ ���� ����ڰ� �����մϴ�.\n";
		mysql_free_result(result);
		return;
	}

	query
		= "INSERT INTO users (name, line, tier, score) VALUES ('" + name + "', '"
		+ line + "', '" + tier + "', " + std::to_string(score) + ");";

	if (!_db->ExecuteQuery(query))
	{
		std::cerr << "�����͸� �����ϴµ� �����߽��ϴ�.\n";
		return;
	}

	std::cout << "������ �߰� �Ϸ�\n";
	mysql_free_result(result);
}