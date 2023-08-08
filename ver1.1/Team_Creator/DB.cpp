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

DB& DB::GetInstance()
{
	// ���α׷��� ��� ��ġ������ ȣ��� �� �ְ� �ϴ� �Լ�

	if (!_instance) // DB Ŭ������ �ν��Ͻ��� ���� �������� �ʾ��� �� �ν��Ͻ� ����
		_instance = new DB();

	return *_instance; // �ν��Ͻ� �����Ͱ� ����Ű�� DB ��ü�� ������ ��ȯ
}

bool DB::IsQuerryError(std::string query)
{
	// ���� ���� ����� ��ȯ�ϴ� �Լ�

	if (mysql_query(&_mysql, query.c_str()) != 0)
	{
		// ���� ������ �߻��� ��� error �޽����� ����ϰ� true�� ��ȯ
		std::cerr << "���� ����: " << mysql_error(&_mysql) << "\n";
		return true;
	}

	return false;
}

void DB::TranslateToMySQL()
{
	// name, line, tier�� �����͸� MySQL�� �����ϴ� �Լ�

	std::string name, line, tier;
	std::cout << "�̸�: ";
	std::cin >> name;

	std::cout << "����(TOP, JUG, MID, AD, SUP): ";
	std::cin >> line;

	std::cout << "Ƽ��(��:M, D1, P1): ";
	std::cin >> tier;

	// MySQL ���̺� ���Ŀ� �°� �̸��� ������ ���ڵ��� ��� �빮�ڷ� ��ȯ
	std::transform(line.begin(), line.end(), line.begin(),
		[](unsigned char ch) { return std::toupper(ch); });
	std::transform(tier.begin(), tier.end(), tier.begin(),
		[](unsigned char ch) { return std::toupper(ch); });

	// Ƽ��ǥ�� �´� ���� �Ҵ�
	int score = _tier_board[tier];

	// �����ͺ��̽��� ������ �����Ͱ� �ִ��� üũ
	// �̶� ������ �������� ������, ������ �̸��� ������ ����

	std::string query
		= "SELECT * FROM users WHERE name = '" + name + " ' AND line = ' " + line + "';";
	// users DB�� ������ name�� line�� ���� ������ ���� ī��Ʈ
	
	if (!IsQuerryError(query))
	{
		MYSQL_RES* res = mysql_store_result(&_mysql); 
		// �ش� ������ ����� ����
		// �� ������ ����� ���� ����� ������ ����Ŵ.

		if (res && mysql_num_rows(res) > 0)
		{
			// mysql_num_rows()�� �ش� ���� ����� ���� ������ ��ȯ
			std::cerr << "�����Ͱ� �̹� �����մϴ�.\n";
			mysql_free_result(res); // ��� ������ �޸𸮿��� ����
			return;
		}

		else
		{
			query
				= "INSERT INTO users (name, line, tier, score) VALUES ('" + name + "', '"
				+ line + "', '" + tier + "', " + std::to_string(score) + ");";
			// MySQL users�� name, line, tier, score �����͸� �߰�

			if (!IsQuerryError(query))
				std::cout << "�����Ͱ� �߰��Ǿ����ϴ�.\n";
		}
	}
}

void DB::ShowDB()
{
	// ���� DB�� ����� �������� ������ ����ϴ� �Լ�

	std::string query
		= "SELECT name, line, tier, score FROM users ORDER BY name;";
	// �̸� ������ ������ ����

	if (!IsQuerryError(query))
	{
		MYSQL_RES* res = mysql_store_result(&_mysql);

		if (res)
		{
			MYSQL_ROW row;
			// MYSQL C API���� ��� ������ �� ���� ��Ÿ���� Ÿ��
			// ���ڿ� �迭�� �ش��ϸ�, �� �ʵ忡 ���� �����͸� ����

			while ((row = mysql_fetch_row(res)))
			{
				// ��� ���տ��� ������ �� �ִ� ���� ���� ���� ��� �ݺ�
				// mysql_fetch_row()�� ��� ���տ��� ���� �࿡ ���� ������

				for (int i = 0; i < mysql_num_fields(res); ++i)
					std::cout << row[i] << "\t";
				// mysql_num_fileds()�� ������ ������� �ʵ��� ���� ��ȯ
				std::cout << "\n";
			}

			mysql_free_result(res);
		}
	}
}