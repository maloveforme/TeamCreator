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

	std::cout << "티어: ";
	std::getline(std::cin, tier);
	score = _tier_board[tier];

	std::cout << "라인: ";
	std::getline(std::cin, line);

	query
		= "SELECT * FROM users WHERE name = '" + name + "' AND line = '" + line + "';";

	MYSQL_RES* result = _db->FetchRows(query);

	if (!result)
	{
		std::cerr << "데이터베이스에서 데이터를 가져오지 못했습니다.\n";
		return;
	}

	if (mysql_num_rows(result) > 0)
	{
		std::cout << "이미 동일한 이름과 라인을 가진 사용자가 존재합니다.\n";
		mysql_free_result(result);
		return;
	}

	query
		= "INSERT INTO users (name, line, tier, score) VALUES ('" + name + "', '"
		+ line + "', '" + tier + "', " + std::to_string(score) + ");";

	if (!_db->ExecuteQuery(query))
	{
		std::cerr << "데이터를 삽입하는데 실패했습니다.\n";
		return;
	}

	std::cout << "데이터 추가 완료\n";
	mysql_free_result(result);
}