#include "DB.h"

DB* DB::_instance = nullptr;
// non-const static 멤버를 클래스 내에서 초기화하는 것이 허용되지 않음.
// 즉, 선언은 클래스 내에서 가능하지만, 초기화는 클래스 밖에서 해야함

DB::DB()
{
	// MYSQL 초기화

	mysql_init(&_mysql); // mysql 시작
	mysql_set_character_set(&_mysql, "utf8"); // 인코딩 utf-8 설정

	// 예외 처리
	if (!mysql_real_connect(&_mysql, "localhost", "root", "malove8466", "TeamCreator", 3306, NULL, 0))
		std::cerr << "MySQL과의 연동에 실패하였습니다: " << mysql_error(&_mysql) << "\n";

	else
		std::cout << "MySQL과의 연동에 성공하였습니다.\n";
}

DB::~DB()
{
	mysql_close(&_mysql); // 메모리 해제
}

DB* DB::GetInstance()
{
	// 프로그램의 어느 위치에서든 호출될 수 있게 하는 함수

	if (!_instance) // DB 클래스의 인스턴스가 아직 생성되지 않았을 때 인스턴스 생성
		_instance = new DB();

	return _instance; // 인스턴스 포인터가 가리키는 DB 객체의 참조를 반환
}

bool DB::ExecuteQuery(const std::string& query)
{
	// 주어진 쿼리 문자열을 MySQL 서버에 전송하고 실행한 후 결과를 true, false로 반환

	return mysql_query(&_mysql, query.c_str()) == 0;
}

MYSQL_RES* DB::FetchRows(const std::string& query)
{
	// 쿼리의 결과를 저장하고 그 결과를 반환하는 함수
	// 쿼리의 실행에 실패하면 에러 메시지를 출력하고 nullptr을 반환

	if (!ExecuteQuery(query))
	{
		std::cerr << "Query Error: " << mysql_error(&_mysql);
		return nullptr;
	}

	// 서버로부터 전체 결과 집합을 가져와 클라이언트의 메모리에 저장
	// 만약 SELECT 쿼리가 아닌 경우 nullptr을 반환함
	return mysql_store_result(&_mysql);
}

std::string DB::GetError() const
{
	return std::string(mysql_error(const_cast<MYSQL*>(&_mysql)));
}

UserRepository::UserRepository()
{
	// 인스턴스 생성

	_db = DB::GetInstance();
}

void UserRepository::PrintUser()
{
	// 데이터베이스 내의 모든 유저 정보를 출력하는 함수

	const std::string query
		= "SELECT name, line, tier, score FROM users ORDER BY name;";

	// 결과를 result 변수에 저장
	MYSQL_RES* result = _db->FetchRows(query);

	if (result)
	{
		// row 변수에 유저 정보를 담아서 하나씩 출력
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(result)))
		{
			for (int i = 0; i < mysql_num_fields(result); ++i)
				std::cout << row[i] << "\t";
			std::cout << "\n";
		}

		// 구조체에 할당된 메모리 해제
		mysql_free_result(result);
	}
}

void UserRepository::AddUser(std::string& name, std::string& tier, std::string& line, int score)
{
	// 유저를 추가하는 함수

	// 동일한 유저가 있는지 검사하는 쿼리
	std::string query
		= "SELECT * FROM users WHERE name = '" + name + "' AND line = '" + line + "';";
	MYSQL_RES* result = _db->FetchRows(query);

	if (!result)
		std::cerr << "데이터베이스에서 데이터를 가져오는 데 실패: " << _db->GetError() << "\n";

	if (mysql_num_rows(result) > 0)
	{
		std::cout << "이미 동일한 이름과 라인을 가진 사용자가 존재합니다.\n";
		return;
	}

	mysql_free_result(result); // 이전 쿼리의 결과를 해제

	// 유저를 추가하는 쿼리
	query = "INSERT INTO users (name, tier, line, score) VALUES ('"
		+ name + "', '" + tier + "', '" + line + "', " + std::to_string(score) + ");";

	if (!_db->ExecuteQuery(query))
	{
		std::cerr << "데이터 삽입 실패: " << _db->GetError() << "\n";
		return;
	}

	std::cout << "데이터 추가 완료\n";
}

void UserRepository::UpdateUsersScore()
{
	const std::string query 
		= "SELECT name, tier, line FROM users;";
	MYSQL_RES* result = _db->FetchRows(query);
	MYSQL_ROW row;

	if (!result) 
	{
		std::cerr << "데이터를 가져오는 데 실패했습니다: " << _db->GetError() << "\n";
		return;
	}

	while ((row = mysql_fetch_row(result))) 
	{
		std::string name = row[0];
		std::string tier = row[1];
		std::string line = row[2];

		int newScore = 0;

		if (Config::tier_score.find(tier) != Config::tier_score.end()) 
			newScore = Config::tier_score[tier];
		
		std::string updateQuery = "UPDATE users SET score=" + std::to_string(newScore) + " WHERE name='" + name + "' AND line='" + line + "';";
		
		if (!_db->ExecuteQuery(updateQuery))
			std::cerr << "점수 업데이트에 실패했습니다: " << _db->GetError() << "\n";

		else
			std::cout << "점수 업데이트에 성공하였습니다\n";
		
	}

	mysql_free_result(result);
}