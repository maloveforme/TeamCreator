#include "DB.h"

DB* DB::_instance = nullptr;
// non-const static ����� Ŭ���� ������ �ʱ�ȭ�ϴ� ���� ������ ����.
// ��, ������ Ŭ���� ������ ����������, �ʱ�ȭ�� Ŭ���� �ۿ��� �ؾ���

DB::DB()
{
	// MYSQL �ʱ�ȭ

	mysql_init(&_mysql); // mysql ����
	mysql_set_character_set(&_mysql, "utf8"); // ���ڵ� utf-8 ����

	// ���� ó��
	if (!mysql_real_connect(&_mysql, "localhost", "root", "malove8466", "TeamCreator", 3306, NULL, 0))
		std::cerr << "MySQL���� ������ �����Ͽ����ϴ�: " << mysql_error(&_mysql) << "\n";

	else
		std::cout << "MySQL���� ������ �����Ͽ����ϴ�.\n";
}

DB::~DB()
{
	mysql_close(&_mysql); // �޸� ����
}

DB* DB::GetInstance()
{
	// ���α׷��� ��� ��ġ������ ȣ��� �� �ְ� �ϴ� �Լ�

	if (!_instance) // DB Ŭ������ �ν��Ͻ��� ���� �������� �ʾ��� �� �ν��Ͻ� ����
		_instance = new DB();

	return _instance; // �ν��Ͻ� �����Ͱ� ����Ű�� DB ��ü�� ������ ��ȯ
}

bool DB::ExecuteQuery(const std::string& query)
{
	// �־��� ���� ���ڿ��� MySQL ������ �����ϰ� ������ �� ����� true, false�� ��ȯ

	return mysql_query(&_mysql, query.c_str()) == 0;
}

MYSQL_RES* DB::FetchRows(const std::string& query)
{
	// ������ ����� �����ϰ� �� ����� ��ȯ�ϴ� �Լ�
	// ������ ���࿡ �����ϸ� ���� �޽����� ����ϰ� nullptr�� ��ȯ

	if (!ExecuteQuery(query))
	{
		std::cerr << "Query Error: " << mysql_error(&_mysql);
		return nullptr;
	}

	// �����κ��� ��ü ��� ������ ������ Ŭ���̾�Ʈ�� �޸𸮿� ����
	// ���� SELECT ������ �ƴ� ��� nullptr�� ��ȯ��
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