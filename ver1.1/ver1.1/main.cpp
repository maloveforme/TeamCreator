#include "play.h"

int main()
{
    std::atexit(DB::CloseMySQL); // �޸� �Ҵ� ����
    Play play;

    play.Start();

    return 0;
}