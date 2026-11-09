#include "game/game.h"

int main(void)
{
    srand((unsigned)time(NULL));
    Game g;
    game_init(&g);
    puts("=== 地牢探险 Roguelike（纯终端 · 第 32～52 章对照）===");
    puts("wasd 移动 | ? 帮助与 DP 演示 | q 退出");
    game_loop(&g);
    game_free(&g);
    return 0;
}
