/*
 * pvz.c —— 终端版《植物大战僵尸》（简化版）
 *
 * 玩法：
 *   - 5 行 x 9 列的草坪，僵尸从右边往左走
 *   - 阳光是货币：向日葵和天上掉下来的阳光都会自动收集（简化）
 *   - 1/2/3/4 选择植物，WASD 或方向键移动光标，空格种植
 *   - 每行左边有一台除草机，僵尸走到尽头时启动，清空一整行（只能用一次）
 *   - 30 只僵尸全部被消灭就胜利；有僵尸在有除草机已用过的行走到尽头就失败
 *
 * 编译：
 *   clang -Wall -Wextra -O2 pvz.c -o pvz
 * 运行：
 *   ./pvz
 *
 * 这个程序把 C 里常用的东西都串了一遍：
 *   结构体 + 二维数组（草坪）、动态数组（僵尸/豌豆）、
 *   时间循环 + 非阻塞读取键盘（termios + select）、ANSI 颜色。
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>
#include <math.h>

/* ---------------- 常量 ---------------- */
#define ROWS 5
#define COLS 9
#define MAX_ZOMBIES 64
#define MAX_PEAS 64
#define TOTAL_ZOMBIES 30

enum { P_EMPTY = 0, P_SUNFLOWER, P_PEASHOOTER, P_WALLNUT, P_REPEATER };
enum { Z_NORMAL = 0, Z_CONE, Z_BUCKET };

/* ---------------- 数据结构 ---------------- */
typedef struct {
    int    type;     /* P_EMPTY 表示空格子 */
    double hp;
    double maxhp;
    double timer;    /* 向日葵产阳光 / 射手开火的倒计时 */
} Plant;

typedef struct {
    int    type;
    int    row;
    double x;        /* 用浮点数表示“第几列”，移动才平滑 */
    double hp, maxhp;
    double speed;    /* 格 / 秒 */
    double dmg;      /* 每秒咬掉的血量 */
    int    eating;
} Zombie;

typedef struct {
    int    row;
    double x;
    double dmg;
} Pea;

/* ---------------- 全局状态 ---------------- */
static Plant  grid[ROWS][COLS];       /* 草坪：每格最多一棵植物 */
static Zombie zombies[MAX_ZOMBIES];
static int    nzombies = 0;
static Pea    peas[MAX_PEAS];
static int    npeas = 0;

static double sun = 150;
static int    mower[ROWS];            /* 1 = 还有除草机，0 = 已经用掉 */
static int    spawned = 0;            /* 已经生成过多少只僵尸 */
static double spawnTimer = 1.0;
static double skyTimer = 8.0;
static double gameTime = 0;

static int paused = 0;
static int gameover = 0;
static int won = 0;
static int quit = 0;

static int selected = P_SUNFLOWER;    /* 当前选中的植物 */
static int cursorRow = 2, cursorCol = 4;

static char message[160] = "按 1-4 选植物，WASD 移动，空格种植";

/* ---------------- ANSI 颜色 ---------------- */
#define CLR_RESET "\x1b[0m"
#define CLR_YEL   "\x1b[33m"
#define CLR_BYEL  "\x1b[93m"
#define CLR_GRN   "\x1b[32m"
#define CLR_BGRN  "\x1b[92m"
#define CLR_RED   "\x1b[91m"
#define CLR_BROWN "\x1b[38;5;130m"
#define CLR_GRAY  "\x1b[90m"
#define CLR_CYAN  "\x1b[96m"

/* ---------------- 小工具 ---------------- */
static double rand01(void) { return (double)rand() / (double)RAND_MAX; }

static double now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

static void msg_set(const char *s)
{
    snprintf(message, sizeof message, "%s", s);
}

static int plant_cost(int t)
{
    switch (t) {
        case P_SUNFLOWER:  return 50;
        case P_PEASHOOTER: return 100;
        case P_WALLNUT:    return 50;
        case P_REPEATER:   return 200;
    }
    return 0;
}

static double plant_hp(int t)
{
    switch (t) {
        case P_SUNFLOWER:  return 120;
        case P_PEASHOOTER: return 120;
        case P_WALLNUT:    return 650;
        case P_REPEATER:   return 120;
    }
    return 100;
}

static const char *plant_name(int t)
{
    switch (t) {
        case P_SUNFLOWER:  return "向日葵";
        case P_PEASHOOTER: return "豌豆射手";
        case P_WALLNUT:    return "坚果墙";
        case P_REPEATER:   return "双发射手";
    }
    return "无";
}

static char plant_char(int t)
{
    switch (t) {
        case P_SUNFLOWER:  return 'S';
        case P_PEASHOOTER: return 'P';
        case P_WALLNUT:    return 'W';
        case P_REPEATER:   return 'R';
    }
    return ' ';
}

static const char *plant_color(int t)
{
    switch (t) {
        case P_SUNFLOWER:  return CLR_YEL;
        case P_PEASHOOTER: return CLR_GRN;
        case P_WALLNUT:    return CLR_BROWN;
        case P_REPEATER:   return CLR_BGRN;
    }
    return CLR_RESET;
}

static char zombie_char(int t)
{
    switch (t) {
        case Z_CONE:   return 'C';
        case Z_BUCKET: return 'B';
    }
    return 'Z';
}

static int count_plants(void)
{
    int n = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (grid[r][c].type != P_EMPTY)
                n++;
    return n;
}

/* ---------------- 终端控制 ---------------- */
static struct termios orig_term;
static int raw_mode_on = 0;

static void restore_term(void)
{
    if (raw_mode_on) {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
        raw_mode_on = 0;
    }
    printf("\x1b[0m\x1b[?25h\n");
    fflush(stdout);
}

static void setup_term(void)
{
    if (!isatty(STDIN_FILENO))
        return;                     /* 被管道重定向时就不折腾终端了 */

    if (tcgetattr(STDIN_FILENO, &orig_term) != 0)
        return;

    struct termios raw = orig_term;
    raw.c_lflag &= ~(ICANON | ECHO | ISIG);   /* 关掉行缓冲、回显、Ctrl+C 信号 */
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == 0) {
        raw_mode_on = 1;
        atexit(restore_term);
        printf("\x1b[?25l");      /* 隐藏光标 */
        fflush(stdout);
    }
}

/* 非阻塞地读键盘：有按键返回字节数，没有返回 0，EOF 返回 -1 */
static int read_keys(unsigned char *buf, int cap)
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    struct timeval tv = {0, 0};
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0)
        return 0;

    int n = (int)read(STDIN_FILENO, buf, (size_t)cap);
    return n > 0 ? n : -1;
}

/* ---------------- 游戏操作 ---------------- */
static void plant_at(int row, int col, int type)
{
    if (type == P_EMPTY) {
        msg_set("先用 1-4 选择一种植物");
        return;
    }
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return;
    if (grid[row][col].type != P_EMPTY) {
        msg_set("这一格已经有植物了");
        return;
    }

    int cost = plant_cost(type);
    if (sun < cost) {
        msg_set("阳光不够，再等等");
        return;
    }

    sun -= cost;
    grid[row][col].type = type;
    grid[row][col].hp = plant_hp(type);
    grid[row][col].maxhp = grid[row][col].hp;
    grid[row][col].timer = (type == P_SUNFLOWER) ? 5.0 : 0.6;

    char buf[96];
    snprintf(buf, sizeof buf, "种下了%s（-%d 阳光）", plant_name(type), cost);
    msg_set(buf);
}

static void remove_at(int row, int col)
{
    if (grid[row][col].type == P_EMPTY) {
        msg_set("这里没有植物可以铲");
        return;
    }
    grid[row][col].type = P_EMPTY;
    grid[row][col].hp = 0;
    msg_set("铲掉了一棵植物");
}

static void spawn_zombie(void)
{
    if (nzombies >= MAX_ZOMBIES)
        return;

    double progress = (double)spawned / TOTAL_ZOMBIES;
    double r = rand01();

    int type = Z_NORMAL;
    if (progress > 0.30 && r < 0.30)
        type = Z_CONE;
    if (progress > 0.60 && r > 0.85)
        type = Z_BUCKET;

    Zombie *z = &zombies[nzombies++];
    memset(z, 0, sizeof *z);
    z->type = type;
    z->row = rand() % ROWS;
    z->x = COLS + 0.8 + rand01() * 0.6;

    switch (type) {
        case Z_CONE:
            z->hp = z->maxhp = 420;
            z->speed = 0.26;
            z->dmg = 25;
            break;
        case Z_BUCKET:
            z->hp = z->maxhp = 700;
            z->speed = 0.23;
            z->dmg = 30;
            break;
        default:
            z->hp = z->maxhp = 180;
            z->speed = 0.26;
            z->dmg = 25;
            break;
    }
}

static int zombie_ahead(int row, int col)
{
    for (int i = 0; i < nzombies; i++)
        if (zombies[i].hp > 0 && zombies[i].row == row && zombies[i].x > (double)col - 0.3)
            return 1;
    return 0;
}

static void add_pea(int row, double x, double dmg)
{
    if (npeas >= MAX_PEAS)
        return;
    peas[npeas].row = row;
    peas[npeas].x = x;
    peas[npeas].dmg = dmg;
    npeas++;
}

/* ---------------- 每帧逻辑 ---------------- */
static void update(double dt)
{
    gameTime += dt;

    /* 天上掉阳光，自动收集 */
    skyTimer -= dt;
    if (skyTimer <= 0) {
        skyTimer = 9.0 + rand01() * 4.0;
        sun += 25;
        msg_set("天降阳光 +25");
    }

    /* 按进度生成僵尸，越到后面越密 */
    if (spawned < TOTAL_ZOMBIES && gameTime > 5.0) {
        spawnTimer -= dt;
        if (spawnTimer <= 0) {
            double progress = (double)spawned / TOTAL_ZOMBIES;
            spawnTimer = 6.0 - 3.5 * progress;

            spawn_zombie();
            spawned++;

            if (spawned == 1)
                msg_set("僵尸来了！");
            else if (spawned == 10)
                msg_set("第 2 波僵尸来袭！");
            else if (spawned == 20)
                msg_set("一大波僵尸正在接近！");
        }
    }

    /* ---- 植物 ---- */
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            Plant *p = &grid[r][c];
            if (p->type == P_EMPTY)
                continue;

            p->timer -= dt;

            if (p->type == P_SUNFLOWER) {
                if (p->timer <= 0) {
                    p->timer += 9.0;
                    sun += 25;
                    msg_set("向日葵产出 25 阳光");
                }
            } else if (p->type == P_PEASHOOTER || p->type == P_REPEATER) {
                if (p->timer <= 0) {
                    if (zombie_ahead(r, c)) {
                        add_pea(r, (double)c + 0.75, 22);
                        if (p->type == P_REPEATER)
                            add_pea(r, (double)c + 0.5, 22);
                        p->timer += 1.4;
                    } else {
                        p->timer = 0.1;   /* 没目标就每 0.1 秒看一眼 */
                    }
                }
            }
        }
    }

    /* ---- 豌豆 ---- */
    for (int i = npeas - 1; i >= 0; i--) {
        peas[i].x += 3.2 * dt;

        int hit = 0;
        for (int j = 0; j < nzombies; j++) {
            if (zombies[j].hp > 0 &&
                zombies[j].row == peas[i].row &&
                fabs(zombies[j].x - peas[i].x) < 0.45) {
                zombies[j].hp -= peas[i].dmg;
                hit = 1;
                break;
            }
        }

        if (hit || peas[i].x > COLS + 1.5) {
            peas[i] = peas[npeas - 1];   /* 用最后一个元素填坑，O(1) 删除 */
            npeas--;
        }
    }

    /* ---- 僵尸 ---- */
    for (int i = nzombies - 1; i >= 0; i--) {
        Zombie *z = &zombies[i];

        if (z->hp <= 0) {
            zombies[i] = zombies[nzombies - 1];
            nzombies--;
            continue;
        }

        int c = (int)floor(z->x);

        /* 脚下这一格有植物就停下来吃 */
        if (c >= 0 && c < COLS && grid[z->row][c].type != P_EMPTY) {
            z->eating = 1;
            grid[z->row][c].hp -= z->dmg * dt;

            if (grid[z->row][c].hp <= 0) {
                grid[z->row][c].type = P_EMPTY;
                msg_set("一棵植物被僵尸吃掉了！");
            }
        } else {
            z->eating = 0;
            z->x -= z->speed * dt;

            if (z->x < -0.15) {
                if (mower[z->row]) {
                    mower[z->row] = 0;
                    for (int j = 0; j < nzombies; j++)
                        if (zombies[j].row == z->row)
                            zombies[j].hp = 0;
                    msg_set("除草机启动，清空了一整行！");
                } else {
                    gameover = 1;
                    won = 0;
                }
            }
        }
    }

    /* ---- 胜负 ---- */
    if (spawned >= TOTAL_ZOMBIES && nzombies == 0) {
        gameover = 1;
        won = 1;
    }
}

/* ---------------- 绘制 ---------------- */
static void draw(void)
{
    /* 光标回到左上角，逐行覆盖重绘（比每次清屏闪烁小） */
    printf("\x1b[H");

    /* 顶栏 */
    printf(CLR_YEL " 阳光: %-4d " CLR_RESET, (int)sun);
    printf(CLR_GRAY "| " CLR_RESET CLR_GRN "植物 %d " CLR_RESET, count_plants());
    printf(CLR_GRAY "| " CLR_RESET CLR_RED "僵尸 %d " CLR_RESET, nzombies);
    printf(CLR_GRAY "| " CLR_RESET "进度 %d/%d " CLR_RESET, spawned, TOTAL_ZOMBIES);
    printf(CLR_GRAY "| " CLR_RESET "时间 %ds " CLR_RESET, (int)gameTime);
    if (paused)
        printf(CLR_CYAN "[暂停]" CLR_RESET);
    printf("\x1b[K\n");

    /* 列号 */
    printf("     ");
    for (int c = 0; c < COLS; c++)
        printf(" %d    ", c + 1);
    printf("\x1b[K\n");

    /* 草坪 */
    for (int r = 0; r < ROWS; r++) {
        printf("    +");
        for (int c = 0; c < COLS; c++)
            printf("-----+");
        printf("\x1b[K\n");

        printf(" %d %c|", r + 1, mower[r] ? '>' : ' ');
        for (int c = 0; c < COLS; c++) {
            char zc = ' ', bc = ' ';

            for (int j = 0; j < nzombies; j++) {
                if (zombies[j].hp > 0 && zombies[j].row == r &&
                    (int)floor(zombies[j].x) == c) {
                    zc = zombie_char(zombies[j].type);
                    break;
                }
            }
            for (int j = 0; j < npeas; j++) {
                if (peas[j].row == r && (int)floor(peas[j].x) == c) {
                    bc = '*';
                    break;
                }
            }

            int t = grid[r][c].type;
            int hl = (r == cursorRow && c == cursorCol);

            printf("%c", hl ? '[' : ' ');

            if (t != P_EMPTY)
                printf("%s%c" CLR_RESET, plant_color(t), plant_char(t));
            else
                printf(" ");

            if (zc != ' ')
                printf(CLR_RED "%c" CLR_RESET, zc);
            else
                printf(" ");

            if (bc != ' ')
                printf(CLR_BYEL "%c" CLR_RESET, bc);
            else
                printf(" ");

            printf("%c|", hl ? ']' : ' ');
        }
        printf("\x1b[K\n");
    }

    printf("    +");
    for (int c = 0; c < COLS; c++)
        printf("-----+");
    printf("\x1b[K\n");

    /* 图例 */
    printf("  " CLR_YEL "S" CLR_RESET "向日葵(50)  "
           CLR_GRN "P" CLR_RESET "豌豆射手(100)  "
           CLR_BROWN "W" CLR_RESET "坚果墙(50)  "
           CLR_BGRN "R" CLR_RESET "双发射手(200)\x1b[K\n");

    printf("  " CLR_RED "Z" CLR_RESET "普通僵尸  "
           CLR_RED "C" CLR_RESET "路障僵尸  "
           CLR_RED "B" CLR_RESET "铁桶僵尸  "
           CLR_BYEL "*" CLR_RESET "豌豆    "
           CLR_GRAY "> 除草机\x1b[K\n");

    /* 状态与操作说明 */
    printf(" 选择: " CLR_CYAN "%s" CLR_RESET "    光标: 第%d行 第%d列\x1b[K\n",
           plant_name(selected), cursorRow + 1, cursorCol + 1);

    printf(" 1-4 选植物 · WASD/方向键 移动 · 空格 种植 · x 铲除 · p 暂停 · q 退出\x1b[K\n");

    printf(" " CLR_GRAY "%s" CLR_RESET "\x1b[K\n", message);

    printf("\x1b[J");   /* 清掉下面多余的旧内容 */
    fflush(stdout);
}

/* ---------------- 键盘处理 ---------------- */
static void handle_keys(const unsigned char *buf, int n)
{
    for (int i = 0; i < n; i++) {
        unsigned char c = buf[i];

        /* 方向键是 ESC [ A/B/C/D */
        if (c == 27 && i + 2 < n && buf[i + 1] == '[') {
            switch (buf[i + 2]) {
                case 'A': if (cursorRow > 0) cursorRow--; break;
                case 'B': if (cursorRow < ROWS - 1) cursorRow++; break;
                case 'C': if (cursorCol < COLS - 1) cursorCol++; break;
                case 'D': if (cursorCol > 0) cursorCol--; break;
            }
            i += 2;
            continue;
        }

        switch (c) {
            case 'q': case 'Q': case 3:   /* 3 = Ctrl+C */
                quit = 1;
                return;
            case 'p': case 'P':
                paused = !paused;
                break;
            case 'w': case 'W':
                if (cursorRow > 0) cursorRow--;
                break;
            case 's': case 'S':
                if (cursorRow < ROWS - 1) cursorRow++;
                break;
            case 'a': case 'A':
                if (cursorCol > 0) cursorCol--;
                break;
            case 'd': case 'D':
                if (cursorCol < COLS - 1) cursorCol++;
                break;
            case '1': selected = P_SUNFLOWER;  break;
            case '2': selected = P_PEASHOOTER; break;
            case '3': selected = P_WALLNUT;    break;
            case '4': selected = P_REPEATER;   break;
            case 'x': case 'X':
                remove_at(cursorRow, cursorCol);
                break;
            case ' ': case '\r': case '\n':
                plant_at(cursorRow, cursorCol, selected);
                break;
        }
    }
}

/* ---------------- 主循环 ---------------- */
int main(void)
{
    srand((unsigned)time(NULL));

    memset(grid, 0, sizeof grid);
    memset(peas, 0, sizeof peas);
    for (int r = 0; r < ROWS; r++)
        mower[r] = 1;

    setup_term();

    double last = now_sec();

    while (!quit) {
        double now = now_sec();
        double dt = now - last;
        last = now;

        if (dt < 0) dt = 0;
        if (dt > 0.2) dt = 0.2;      /* 电脑卡了一下也不让游戏瞬移 */

        unsigned char buf[16];
        int n = read_keys(buf, (int)sizeof buf);
        if (n < 0)
            break;                    /* stdin 关闭 */
        if (n > 0) {
            handle_keys(buf, n);
            if (quit)
                break;
        }

        if (!paused && !gameover)
            update(dt);

        draw();

        if (gameover)
            break;

        struct timespec ts = {0, 50000000L};   /* 50ms ≈ 20 FPS */
        nanosleep(&ts, NULL);
    }

    restore_term();

    if (gameover) {
        if (won)
            printf("\n 胜利！30 只僵尸全部被消灭，草坪守住了。\n");
        else
            printf("\n 僵尸吃掉了你的脑子…… 游戏结束。\n");
    } else {
        printf("\n 已退出游戏。\n");
    }

    return 0;
}
