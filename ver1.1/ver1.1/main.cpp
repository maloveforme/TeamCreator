#include "play.h"

int main()
{
    std::atexit(DB::CloseMySQL); // 메모리 할당 해제
    Play play;

    play.Start();

    return 0;
}