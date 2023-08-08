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

DB& DB::GetInstance()
{
	// 프로그램의 어느 위치에서든 호출될 수 있게 하는 함수

	if (!_instance) // DB 클래스의 인스턴스가 아직 생성되지 않았을 때 인스턴스 생성
		_instance = new DB();

	return *_instance; // 인스턴스 포인터가 가리키는 DB 객체의 참조를 반환
}

bool DB::IsQuerryError(std::string query)
{
	// 쿼리 에러 결과를 반환하는 함수

	if (mysql_query(&_mysql, query.c_str()) != 0)
	{
		// 쿼리 오류가 발생한 경우 error 메시지를 출력하고 true값 반환
		std::cerr << "쿼리 오류: " << mysql_error(&_mysql) << "\n";
		return true;
	}

	return false;
}

void DB::TranslateToMySQL()
{
	// name, line, tier의 데이터를 MySQL에 전송하는 함수

	std::string name, line, tier;
	std::cout << "이름: ";
	std::cin >> name;

	std::cout << "라인(TOP, JUG, MID, AD, SUP): ";
	std::cin >> line;

	std::cout << "티어(예:M, D1, P1): ";
	std::cin >> tier;

	// MySQL 테이블 형식에 맞게 이름을 제외한 문자들을 모두 대문자로 변환
	std::transform(line.begin(), line.end(), line.begin(),
		[](unsigned char ch) { return std::toupper(ch); });
	std::transform(tier.begin(), tier.end(), tier.begin(),
		[](unsigned char ch) { return std::toupper(ch); });

	// 티어표에 맞는 점수 할당
	int score = _tier_board[tier];

	// 데이터베이스에 동일한 데이터가 있는지 체크
	// 이때 동일한 데이터의 기준은, 동일한 이름과 동일한 라인

	std::string query
		= "SELECT * FROM users WHERE name = '" + name + " ' AND line = ' " + line + "';";
	// users DB에 동일한 name과 line을 가진 유저의 수를 카운트
	
	if (!IsQuerryError(query))
	{
		MYSQL_RES* res = mysql_store_result(&_mysql); 
		// 해당 쿼리의 결과를 저장
		// 이 형식은 결과로 얻은 행들의 집합을 가리킴.

		if (res && mysql_num_rows(res) > 0)
		{
			// mysql_num_rows()는 해당 쿼리 결과의 행의 개수를 반환
			std::cerr << "데이터가 이미 존재합니다.\n";
			mysql_free_result(res); // 결과 집합을 메모리에서 해제
			return;
		}

		else
		{
			query
				= "INSERT INTO users (name, line, tier, score) VALUES ('" + name + "', '"
				+ line + "', '" + tier + "', " + std::to_string(score) + ");";
			// MySQL users에 name, line, tier, score 데이터를 추가

			if (!IsQuerryError(query))
				std::cout << "데이터가 추가되었습니다.\n";
		}
	}
}

void DB::ShowDB()
{
	// 현재 DB에 저장된 유저들의 정보를 출력하는 함수

	std::string query
		= "SELECT name, line, tier, score FROM users ORDER BY name;";
	// 이름 순으로 정렬한 쿼리

	if (!IsQuerryError(query))
	{
		MYSQL_RES* res = mysql_store_result(&_mysql);

		if (res)
		{
			MYSQL_ROW row;
			// MYSQL C API에서 결과 집합의 한 행을 나타내는 타입
			// 문자열 배열에 해당하며, 각 필드에 대한 데이터를 포함

			while ((row = mysql_fetch_row(res)))
			{
				// 결과 집합에서 가져올 수 있는 행이 있을 동안 계속 반복
				// mysql_fetch_row()는 결과 집합에서 다음 행에 대한 데이터

				for (int i = 0; i < mysql_num_fields(res); ++i)
					std::cout << row[i] << "\t";
				// mysql_num_fileds()는 쿼리의 결과에서 필드의 수를 반환
				std::cout << "\n";
			}

			mysql_free_result(res);
		}
	}
}