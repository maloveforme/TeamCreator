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
	// 어디서든지 해당 클래스를 호출하는 함수

	if (!_instance)
		_instance = new Board();

	return *_instance;
}

void Board::ShowBoard()
{
	// 현재 5:5 현황판을 A팀과 B팀을 기준으로 출력

	std::cout << "A팀 " << std::setw(4) << _total_score[0] << "     B팀 " << std::setw(4) << _total_score[1] << "\n";

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

	std::cout << "유저를 보드에 넣습니다\n";
	std::cout << "이름: ";
	std::getline(std::cin, name);

	std::cout << "라인: ";
	std::getline(std::cin, line);

	


}

void Board::ChangeUser()
{

}