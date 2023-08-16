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

std::string DB::GetError() const
{
	return std::string(mysql_error(const_cast<MYSQL*>(&_mysql)));
}

UserRepository::UserRepository()
{
	// �ν��Ͻ� ����

	_db = DB::GetInstance();
}

void UserRepository::PrintUser()
{
	// �����ͺ��̽� ���� ��� ���� ������ ����ϴ� �Լ�

	const std::string query
		= "SELECT name, line, tier, score FROM users ORDER BY name;";

	// ����� result ������ ����
	MYSQL_RES* result = _db->FetchRows(query);

	if (result)
	{
		// row ������ ���� ������ ��Ƽ� �ϳ��� ���
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(result)))
		{
			for (int i = 0; i < mysql_num_fields(result); ++i)
				std::cout << row[i] << "\t";
			std::cout << "\n";
		}

		// ����ü�� �Ҵ�� �޸� ����
		mysql_free_result(result);
	}
}

void UserRepository::AddUser(std::string& name, std::string& tier, std::string& line, int score)
{
	// ������ �߰��ϴ� �Լ�

	// ������ ������ �ִ��� �˻��ϴ� ����
	std::string query
		= "SELECT * FROM users WHERE name = '" + name + "' AND line = '" + line + "';";
	MYSQL_RES* result = _db->FetchRows(query);

	if (!result)
		std::cerr << "�����ͺ��̽����� �����͸� �������� �� ����: " << _db->GetError() << "\n";

	if (mysql_num_rows(result) > 0)
	{
		std::cout << "�̹� ������ �̸��� ������ ���� ����ڰ� �����մϴ�.\n";
		return;
	}

	mysql_free_result(result); // ���� ������ ����� ����

	// ������ �߰��ϴ� ����
	query = "INSERT INTO users (name, tier, line, score) VALUES ('"
		+ name + "', '" + tier + "', '" + line + "', " + std::to_string(score) + ");";

	if (!_db->ExecuteQuery(query))
	{
		std::cerr << "������ ���� ����: " << _db->GetError() << "\n";
		return;
	}

	std::cout << "������ �߰� �Ϸ�\n";
}

void UserRepository::UpdateUsersScore()
{
	const std::string query 
		= "SELECT name, tier, line FROM users;";
	MYSQL_RES* result = _db->FetchRows(query);
	MYSQL_ROW row;

	if (!result) 
	{
		std::cerr << "�����͸� �������� �� �����߽��ϴ�: " << _db->GetError() << "\n";
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
			std::cerr << "���� ������Ʈ�� �����߽��ϴ�: " << _db->GetError() << "\n";

		else
			std::cout << "���� ������Ʈ�� �����Ͽ����ϴ�\n";
		
	}

	mysql_free_result(result);
}