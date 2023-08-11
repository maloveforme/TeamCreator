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

UserRepository::UserRepository()
{
	_db = DB::GetInstance();
}

void UserRepository::PrintUser()
{
	const std::string query
		= "SELECT name, line, tier, score FROM users ORDER BY name;";
	MYSQL_RES* result = _db->FetchRows(query);

	if (result)
	{
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(result)))
		{
			for (int i = 0; i < mysql_num_fields(result); ++i)
				std::cout << row[i] << "\t";
			std::cout << "\n";
		}

		mysql_free_result(result);
	}
}