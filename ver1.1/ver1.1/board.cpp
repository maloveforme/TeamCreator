#include "board.h"

Board* Board::_instance = nullptr;

Board::Board() : _total_score{ 0, 0 }, _board{}
{
	
}

Board::~Board()
{

}

Board& Board::GetInstance()
{
	// ��𼭵��� �ش� Ŭ������ ȣ���ϴ� �Լ�

	if (!_instance)
		_instance = new Board();

	return *_instance;
}

void Board::ShowBoard()
{
	// ���� 5:5 ��Ȳ���� A���� B���� �������� ���

	std::cout << "A�� " << std::setw(4) << _total_score[0] << "     B�� " << std::setw(4) << _total_score[1] << "\n";

	for (const auto& b : _board)
	{
		std::cout << b[0].first << " " << b[0].second << "  ";
		std::cout << b[1].first << " " << b[1].second << "  \n";
	}
}

void Board::SetUser()
{
	std::string name, line;
	char team;

	std::cout << "������ ���忡 �ֽ��ϴ�\n";
	std::cout << "�̸�: ";
	std::getline(std::cin, name);

	std::cout << "����: ";
	std::getline(std::cin, line);

	


}

void Board::ChangeUser()
{

}