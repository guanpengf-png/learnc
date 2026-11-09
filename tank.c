/*
 * tank.c —— 终端版坦克大战（Battle City）
 * 编译: cc -O2 -Wall -Wextra -o tank tank.c && ./tank      (macOS / Linux，无需任何库)
 * 操作: WASD 或方向键移动，空格开火，Q 退出
 *
 * ┌ 知识点索引 ──────────────────────────────────────────────────────────┐
 *  指针       : 二级指针删链表(update_bullets)、指针网格 occ[][]、指向格子的指针 Tile*、
 *               函数指针 AIFunc(敌人"大脑")、指针传出参数 Vec2 **out、const 指针、
 *               指针算术 (print_report 里遍历数组)
 *  内存管理   : malloc/realloc/free、动态数组(tanks)、realloc 后指针失效要重建(rebuild_occ)
 *  数据结构   : 单向链表(子弹)、动态数组(坦克池)、二叉最小堆/优先队列(A*)、
 *               队列(BFS)、二维数组(地图)、结构体/枚举/位标志
 *  算法       : A* 加权寻路、BFS 寻路、曼哈顿距离启发函数、视线扫描、qsort 排序
 *  其它       : 指定初始化器、复合字面量、static inline、宏、termios 原始终端、信号
 * └───────────────────────────────────────────────────────────────────────┘
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define W 31
#define H 21
#define TOTAL_ENEMY 12
#define MAX_ALIVE   4
#define TICK_NS     80000000L /* 一帧 80ms */
#define INF         0x3f3f3f3f

/* ---------- 基础类型 ---------- */
typedef struct { int x, y; } Vec2;
typedef enum { UP, RIGHT, DOWN, LEFT } Dir;
static const int DX[4] = {0, 1, 0, -1}, DY[4] = {-1, 0, 1, 0};
static const char *DIR_GLYPH[4] = {"^^", ">>", "vv", "<<"};

typedef enum { EMPTY, BRICK, STEEL, WATER, GRASS, BASE, TILE_COUNT } Tile;
/* 位标志：每种地形的属性用一个整数的不同位表示 */
enum { F_BLOCK_TANK = 1, F_BLOCK_BULLET = 2, F_BREAKABLE = 4 };
static const unsigned TILE_FLAGS[TILE_COUNT] = { /* 指定初始化器 */
    [EMPTY] = 0,
    [BRICK] = F_BLOCK_TANK | F_BLOCK_BULLET | F_BREAKABLE,
    [STEEL] = F_BLOCK_TANK | F_BLOCK_BULLET,
    [WATER] = F_BLOCK_TANK,
    [GRASS] = 0,
    [BASE]  = F_BLOCK_TANK | F_BLOCK_BULLET,
};
static const struct { const char *glyph; int color; } TILE_GFX[TILE_COUNT] = {
    [EMPTY] = {"  ", 0},  [BRICK] = {"▒▒", 31}, [STEEL] = {"██", 37},
    [WATER] = {"~~", 34}, [GRASS] = {"\"\"", 32}, [BASE] = {"BB", 93},
};

typedef struct Game Game;
typedef struct Tank Tank;
typedef void (*AIFunc)(Game *, Tank *); /* 函数指针类型：敌人 AI */

typedef enum { K_WANDER, K_HUNTER, K_RAIDER, KIND_COUNT } Kind;

struct Tank {
    Vec2 pos; Dir dir; int hp; bool alive;
    int team;                    /* 0 玩家, 1 敌人 */
    Kind kind;
    int cooldown, move_cd, move_period, repath;
    AIFunc think;                /* 每个坦克自带一个函数指针 */
    Vec2 *path; int plen, pidx;  /* 动态分配的路径数组 */
};

typedef struct Bullet {          /* 单向链表节点 */
    Vec2 pos; Dir dir; int team; bool dead;
    struct Bullet *next;
} Bullet;

struct Game {
    Tile map[H][W];
    Tank *tanks; size_t ntanks, cap;   /* 动态数组，tanks[0] 是玩家 */
    Tank *occ[H][W];                   /* 指针网格：格子上是哪辆坦克，O(1) 查询 */
    Bullet *bullets;                   /* 链表头 */
    Vec2 base;
    int score, lives, tick, spawned, alive_enemies, respawn_cd;
    int kills[KIND_COUNT];
    bool base_alive, over, won, quit;
};

/* ---------- 工具 ---------- */
static void *xrealloc(void *p, size_t n) {
    void *q = realloc(p, n);
    if (!q) { fputs("out of memory\n", stderr); exit(1); }
    return q;
}
static inline bool in_bounds(int x, int y) { return x >= 0 && x < W && y >= 0 && y < H; }
static inline bool same(Vec2 a, Vec2 b) { return a.x == b.x && a.y == b.y; }
static inline int manhattan(Vec2 a, Vec2 b) { return abs(a.x - b.x) + abs(a.y - b.y); }
static Dir dir_between(Vec2 a, Vec2 b) {
    if (b.x > a.x) return RIGHT;
    if (b.x < a.x) return LEFT;
    return b.y > a.y ? DOWN : UP;
}

/* ---------- 最小堆（优先队列）：A* 的核心 ---------- */
typedef struct { int idx, f; } HNode;
typedef struct { HNode *a; int n, cap; } Heap;

static void heap_push(Heap *h, HNode x) {
    if (h->n == h->cap) { h->cap = h->cap ? h->cap * 2 : 64; h->a = xrealloc(h->a, h->cap * sizeof *h->a); }
    int i = h->n++;
    while (i > 0) {                       /* 上浮 */
        int p = (i - 1) / 2;
        if (h->a[p].f <= x.f) break;
        h->a[i] = h->a[p]; i = p;
    }
    h->a[i] = x;
}
static HNode heap_pop(Heap *h) {
    HNode top = h->a[0], last = h->a[--h->n];
    int i = 0;
    for (;;) {                            /* 下沉 */
        int c = 2 * i + 1;
        if (c >= h->n) break;
        if (c + 1 < h->n && h->a[c + 1].f < h->a[c].f) c++;
        if (last.f <= h->a[c].f) break;
        h->a[i] = h->a[c]; i = c;
    }
    if (h->n > 0) h->a[i] = last;
    return top;
}

/* ---------- 寻路 ---------- */
/* 地形代价：空地 1，砖墙 4（要先打掉，更慢），钢/水不可通行(-1) */
static int tile_cost(Tile t) {
    switch (t) {
        case EMPTY: case GRASS: case BASE: return 1;
        case BRICK: return 4;
        default: return -1;
    }
}
/* 由 parent[] 回溯出路径。通过二级指针 out 把 malloc 的数组"传出"给调用者 */
static int build_path(const int *parent, int ti, Vec2 **out) {
    if (parent[ti] == -2) return 0;       /* 没访问到 = 不可达 */
    int len = 0;
    for (int i = ti; i != -1; i = parent[i]) len++;
    Vec2 *p = xrealloc(NULL, len * sizeof *p);
    int k = len - 1;
    for (int i = ti; i != -1; i = parent[i]) p[k--] = (Vec2){i % W, i / W};
    *out = p;
    return len;
}
static int astar(const Game *g, Vec2 s, Vec2 t, Vec2 **out) {
    int cost[W * H], parent[W * H];
    bool closed[W * H] = {false};
    for (int i = 0; i < W * H; i++) { cost[i] = INF; parent[i] = -2; }
    int si = s.y * W + s.x, ti = t.y * W + t.x;
    cost[si] = 0; parent[si] = -1;
    Heap h = {0};
    heap_push(&h, (HNode){si, manhattan(s, t)});
    while (h.n > 0) {
        HNode cur = heap_pop(&h);
        if (closed[cur.idx]) continue;
        closed[cur.idx] = true;
        if (cur.idx == ti) break;
        for (int d = 0; d < 4; d++) {
            int nx = cur.idx % W + DX[d], ny = cur.idx / W + DY[d];
            if (!in_bounds(nx, ny)) continue;
            int c = tile_cost(g->map[ny][nx]);
            if (c < 0) continue;
            int ni = ny * W + nx, ng = cost[cur.idx] + c;
            if (ng < cost[ni]) {
                cost[ni] = ng; parent[ni] = cur.idx;
                heap_push(&h, (HNode){ni, ng + manhattan((Vec2){nx, ny}, t)});
            }
        }
    }
    free(h.a);
    return build_path(parent, ti, out);
}
/* BFS：不看代价，找步数最少的路（队列用数组 + 头尾下标实现） */
static int bfs(const Game *g, Vec2 s, Vec2 t, Vec2 **out) {
    int parent[W * H], queue[W * H], head = 0, tail = 0;
    for (int i = 0; i < W * H; i++) parent[i] = -2;
    int si = s.y * W + s.x, ti = t.y * W + t.x;
    parent[si] = -1; queue[tail++] = si;
    while (head < tail) {
        int cur = queue[head++];
        if (cur == ti) break;
        for (int d = 0; d < 4; d++) {
            int nx = cur % W + DX[d], ny = cur / W + DY[d];
            if (!in_bounds(nx, ny) || tile_cost(g->map[ny][nx]) < 0) continue;
            int ni = ny * W + nx;
            if (parent[ni] != -2) continue;
            parent[ni] = cur; queue[tail++] = ni;
        }
    }
    return build_path(parent, ti, out);
}

/* ---------- 坦克与子弹 ---------- */
static bool can_enter(const Game *g, int x, int y) {
    return in_bounds(x, y) && !(TILE_FLAGS[g->map[y][x]] & F_BLOCK_TANK) && g->occ[y][x] == NULL;
}
static bool try_move(Game *g, Tank *t, Dir d) {
    t->dir = d;
    int nx = t->pos.x + DX[d], ny = t->pos.y + DY[d];
    if (!can_enter(g, nx, ny)) return false;
    Tank **slot = &g->occ[t->pos.y][t->pos.x];  /* 指向网格槽位的指针 */
    *slot = NULL;
    t->pos = (Vec2){nx, ny};
    g->occ[ny][nx] = t;
    return true;
}
static void fire(Game *g, Tank *t) {
    if (t->cooldown > 0) return;
    Bullet *b = xrealloc(NULL, sizeof *b);
    *b = (Bullet){.pos = t->pos, .dir = t->dir, .team = t->team, .dead = false, .next = g->bullets};
    g->bullets = b;                              /* 头插法 */
    t->cooldown = t->team == 0 ? 4 : 10;
}
static void kill_tank(Game *g, Tank *t);

static void update_bullets(Game *g) {
    for (Bullet *b = g->bullets; b; b = b->next) {
        if (b->dead) continue;
        int nx = b->pos.x + DX[b->dir], ny = b->pos.y + DY[b->dir];
        if (!in_bounds(nx, ny)) { b->dead = true; continue; }
        b->pos = (Vec2){nx, ny};
        Tank *t = g->occ[ny][nx];
        if (t && t->team != b->team) {
            b->dead = true;
            if (--t->hp <= 0) kill_tank(g, t);
            continue;
        }
        Tile *tile = &g->map[ny][nx];            /* 直接拿到格子的指针，可读可写 */
        unsigned f = TILE_FLAGS[*tile];
        if (*tile == BASE) { g->base_alive = false; g->over = true; b->dead = true; }
        else if (f & F_BLOCK_BULLET) { b->dead = true; if (f & F_BREAKABLE) *tile = EMPTY; }
    }
    /* 敌我子弹相撞互相抵消：链表上的 O(n²) 两两比较 */
    for (Bullet *a = g->bullets; a; a = a->next)
        for (Bullet *c = a->next; c; c = c->next)
            if (!a->dead && !c->dead && a->team != c->team && same(a->pos, c->pos))
                a->dead = c->dead = true;
    /* 二级指针删除链表节点：不需要记 prev，也不用特判头节点 */
    Bullet **pp = &g->bullets;
    while (*pp) {
        Bullet *b = *pp;
        if (b->dead) { *pp = b->next; free(b); }
        else pp = &b->next;
    }
}

/* ---------- AI：三种"大脑"，都是 AIFunc ---------- */
static void ai_wander(Game *g, Tank *t);
static void ai_hunter(Game *g, Tank *t);
static void ai_raider(Game *g, Tank *t);

static const struct { const char *name; AIFunc think; int hp, period, color, points; } SPEC[KIND_COUNT] = {
    [K_WANDER] = {"游荡者(随机)", ai_wander, 1, 5, 97, 100},
    [K_HUNTER] = {"猎手(A*追玩家)", ai_hunter, 2, 4, 95, 200},
    [K_RAIDER] = {"突击兵(BFS攻基地)", ai_raider, 1, 4, 96, 150},
};

/* 沿当前朝向扫描，第一个碰到的东西是玩家或基地吗？（视线判定） */
static bool sees_target(const Game *g, const Tank *t) {
    int x = t->pos.x, y = t->pos.y;
    for (;;) {
        x += DX[t->dir]; y += DY[t->dir];
        if (!in_bounds(x, y)) return false;
        const Tank *o = g->occ[y][x];
        if (o && o->team == 0) return true;
        Tile tl = g->map[y][x];
        if (tl == BASE) return true;
        if (TILE_FLAGS[tl] & F_BLOCK_BULLET) return false;
    }
}
static void repath(Game *g, Tank *t, Vec2 goal, bool use_astar) {
    free(t->path); t->path = NULL; t->pidx = 0;
    t->plen = use_astar ? astar(g, t->pos, goal, &t->path) : bfs(g, t->pos, goal, &t->path);
}
/* 沿路径走一步；下一格是砖墙就先开炮打掉 */
static void follow_path(Game *g, Tank *t) {
    while (t->pidx < t->plen && same(t->path[t->pidx], t->pos)) t->pidx++;
    if (t->pidx >= t->plen) return;
    Vec2 n = t->path[t->pidx];
    t->dir = dir_between(t->pos, n);
    if (g->map[n.y][n.x] == BRICK) fire(g, t);
    else try_move(g, t, t->dir);
}
static void ai_wander(Game *g, Tank *t) {
    if (rand() % 5 == 0) t->dir = (Dir)(rand() % 4);
    if (!try_move(g, t, t->dir)) t->dir = (Dir)(rand() % 4);
    if (sees_target(g, t) || rand() % 6 == 0) fire(g, t);
}
static void ai_hunter(Game *g, Tank *t) {
    Tank *p = &g->tanks[0];
    if (!p->alive) { ai_wander(g, t); return; }
    if (--t->repath <= 0 || t->pidx >= t->plen) { repath(g, t, p->pos, true); t->repath = 5; }
    if (sees_target(g, t)) { fire(g, t); return; }
    follow_path(g, t);
}
static void ai_raider(Game *g, Tank *t) {
    if (--t->repath <= 0 || t->pidx >= t->plen) { repath(g, t, g->base, false); t->repath = 8; }
    if (sees_target(g, t)) { fire(g, t); return; }
    follow_path(g, t);
}

/* ---------- 游戏状态 ---------- */
static void rebuild_occ(Game *g) {
    memset(g->occ, 0, sizeof g->occ);
    for (size_t i = 0; i < g->ntanks; i++)
        if (g->tanks[i].alive) g->occ[g->tanks[i].pos.y][g->tanks[i].pos.x] = &g->tanks[i];
}
static void kill_tank(Game *g, Tank *t) {
    t->alive = false;
    g->occ[t->pos.y][t->pos.x] = NULL;
    free(t->path); t->path = NULL; t->plen = 0;
    if (t->team == 1) { g->score += SPEC[t->kind].points; g->kills[t->kind]++; g->alive_enemies--; }
    else if (--g->lives <= 0) g->over = true;
    else g->respawn_cd = 15;
}
static Tank *alloc_tank(Game *g) {
    for (size_t i = 1; i < g->ntanks; i++)       /* 先复用死坦克的槽位 */
        if (!g->tanks[i].alive) return &g->tanks[i];
    if (g->ntanks == g->cap) {
        g->cap *= 2;
        g->tanks = xrealloc(g->tanks, g->cap * sizeof *g->tanks);
        rebuild_occ(g);  /* realloc 可能搬家，旧的 Tank* 全部失效，必须重建！ */
    }
    return &g->tanks[g->ntanks++];
}
static void spawn_enemy(Game *g) {
    static const Vec2 SP[3] = {{1, 1}, {W / 2, 1}, {W - 2, 1}};
    Vec2 p = SP[g->spawned % 3];
    if (g->occ[p.y][p.x]) return;
    Tank *t = alloc_tank(g);
    Kind k = (Kind)(rand() % KIND_COUNT);
    *t = (Tank){.pos = p, .dir = DOWN, .hp = SPEC[k].hp, .alive = true, .team = 1, .kind = k,
                .move_cd = SPEC[k].period, .move_period = SPEC[k].period, .think = SPEC[k].think};
    g->occ[p.y][p.x] = t;
    g->spawned++; g->alive_enemies++;
}
static void fill(Game *g, int x0, int y0, int w, int h, Tile t) {
    for (int y = y0; y < y0 + h; y++) for (int x = x0; x < x0 + w; x++) g->map[y][x] = t;
}
static void game_init(Game *g) {
    memset(g, 0, sizeof *g);
    fill(g, 0, 0, W, 1, STEEL); fill(g, 0, H - 1, W, 1, STEEL);
    fill(g, 0, 0, 1, H, STEEL); fill(g, W - 1, 0, 1, H, STEEL);
    for (int y = 3; y < H - 5; y += 4) for (int x = 3; x < W - 3; x += 4) fill(g, x, y, 2, 2, BRICK);
    fill(g, 8, 9, 5, 1, WATER); fill(g, 18, 9, 5, 1, WATER); fill(g, 15, 9, 1, 2, STEEL);
    fill(g, 5, 13, 4, 2, GRASS);
    fill(g, W / 2 - 1, H - 3, 3, 1, BRICK); fill(g, W / 2 - 1, H - 2, 1, 1, BRICK); fill(g, W / 2 + 1, H - 2, 1, 1, BRICK);
    g->base = (Vec2){W / 2, H - 2};
    g->map[g->base.y][g->base.x] = BASE;
    g->base_alive = true; g->lives = 3;
    g->cap = 4; g->ntanks = 1;
    g->tanks = xrealloc(NULL, g->cap * sizeof *g->tanks);
    g->tanks[0] = (Tank){.pos = {W / 2 - 4, H - 2}, .dir = UP, .hp = 1, .alive = true, .team = 0};
    rebuild_occ(g);
}

/* ---------- 输入 / 更新 ---------- */
static void handle_input(Game *g) {
    unsigned char buf[64];
    ssize_t n = read(STDIN_FILENO, buf, sizeof buf);
    int want = -1; bool shoot = false;
    for (ssize_t i = 0; i < n; i++) {
        int c = buf[i];
        if (c == 27 && i + 2 < n && buf[i + 1] == '[') {      /* 方向键 ESC [ A/B/C/D */
            switch (buf[i + 2]) { case 'A': want = UP; break; case 'C': want = RIGHT; break;
                                  case 'B': want = DOWN; break; case 'D': want = LEFT; break; }
            i += 2; continue;
        }
        switch (c) {
            case 'w': want = UP; break;    case 'd': want = RIGHT; break;
            case 's': want = DOWN; break;  case 'a': want = LEFT; break;
            case ' ': shoot = true; break; case 'q': g->quit = true; break;
        }
    }
    Tank *p = &g->tanks[0];
    if (!p->alive) return;
    if (want >= 0) {
        if (p->move_cd <= 0) { try_move(g, p, (Dir)want); p->move_cd = 2; }
        else p->dir = (Dir)want;
    }
    if (shoot) fire(g, p);
}
static void update(Game *g) {
    g->tick++;
    Tank *p = &g->tanks[0];
    if (!p->alive && g->lives > 0 && --g->respawn_cd <= 0 && !g->occ[H - 2][W / 2 - 4]) {
        *p = (Tank){.pos = {W / 2 - 4, H - 2}, .dir = UP, .hp = 1, .alive = true, .team = 0};
        g->occ[p->pos.y][p->pos.x] = p;
    }
    if (g->spawned < TOTAL_ENEMY && g->alive_enemies < MAX_ALIVE && g->tick % 25 == 0) spawn_enemy(g);
    for (size_t i = 0; i < g->ntanks; i++) {
        Tank *t = &g->tanks[i];
        if (!t->alive) continue;
        if (t->cooldown > 0) t->cooldown--;
        if (t->team == 0) { if (t->move_cd > 0) t->move_cd--; continue; }
        if (--t->move_cd <= 0) { t->move_cd = t->move_period; t->think(g, t); } /* 通过函数指针调用 AI */
    }
    update_bullets(g);
    if (g->spawned == TOTAL_ENEMY && g->alive_enemies == 0) g->won = true;
}

/* ---------- 渲染 ---------- */
static void render(const Game *g) {
    static char out[1 << 16];
    bool bul[H][W]; memset(bul, 0, sizeof bul);
    for (const Bullet *b = g->bullets; b; b = b->next) bul[b->pos.y][b->pos.x] = true;
    char *p = out;                                    /* 用指针当"写入游标"拼整帧，一次 fwrite */
    p += sprintf(p, "\033[H\033[0m得分 %-5d 生命 %d  敌人 %2d/%d  [WASD移动 空格开火 Q退出]\033[K\n",
                 g->score, g->lives, TOTAL_ENEMY - g->spawned + g->alive_enemies, TOTAL_ENEMY);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            const Tank *t = g->occ[y][x];
            Tile tl = g->map[y][x];
            const char *glyph = TILE_GFX[tl].glyph; int color = TILE_GFX[tl].color;
            if (tl != GRASS) {                        /* 草丛盖住坦克和子弹 */
                if (t) { glyph = DIR_GLYPH[t->dir]; color = t->team == 0 ? 93 : SPEC[t->kind].color; }
                else if (bul[y][x]) { glyph = "**"; color = 97; }
                else if (tl == BASE && !g->base_alive) { glyph = "xx"; color = 31; }
            }
            p += sprintf(p, "\033[%dm%s", color, glyph);
        }
        p += sprintf(p, "\033[0m\n");
    }
    fwrite(out, 1, (size_t)(p - out), stdout);
    fflush(stdout);
}

/* ---------- 终端 ---------- */
static struct termios orig_term;
static bool term_on;
static volatile sig_atomic_t stop_flag;
static void term_restore(void) {
    if (!term_on) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
    fputs("\033[?25h\033[?1049l", stdout); fflush(stdout);
    term_on = false;
}
static void on_sig(int s) { (void)s; stop_flag = 1; }
static void term_init(void) {
    if (tcgetattr(STDIN_FILENO, &orig_term) == 0) {
        struct termios raw = orig_term;
        raw.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
        raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 0;      /* read() 不阻塞 */
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }
    term_on = true;
    atexit(term_restore);
    signal(SIGINT, on_sig);
    fputs("\033[?1049h\033[?25l\033[2J", stdout);
}

/* ---------- 战报：qsort + 比较函数指针 ---------- */
typedef struct { const char *name; int kills, pts; } Row;
static int cmp_row(const void *a, const void *b) {
    const Row *x = a, *y = b;                         /* void* 转回具体类型 */
    return (y->kills * y->pts) - (x->kills * x->pts);
}
static void print_report(const Game *g) {
    Row rows[KIND_COUNT];
    for (Row *r = rows; r < rows + KIND_COUNT; r++) {  /* 指针算术遍历数组 */
        int k = (int)(r - rows);
        *r = (Row){SPEC[k].name, g->kills[k], SPEC[k].points};
    }
    qsort(rows, KIND_COUNT, sizeof rows[0], cmp_row);
    printf("\n====== %s ======\n总分: %d\n击杀战报(按得分排序):\n",
           g->won ? "胜利！" : "游戏结束", g->score);
    for (int i = 0; i < KIND_COUNT; i++)
        printf("  %-20s x%d  = %d 分\n", rows[i].name, rows[i].kills, rows[i].kills * rows[i].pts);
}

int main(void) {
    srand((unsigned)time(NULL));
    static Game g;
    game_init(&g);
    term_init();
    struct timespec ts = {0, TICK_NS};
    while (!g.quit && !g.over && !g.won && !stop_flag) {
        handle_input(&g); update(&g); render(&g);
        nanosleep(&ts, NULL);
    }
    render(&g);
    term_restore();
    print_report(&g);
    /* 释放所有堆内存 */
    for (size_t i = 0; i < g.ntanks; i++) free(g.tanks[i].path);
    free(g.tanks);
    while (g.bullets) { Bullet *n = g.bullets->next; free(g.bullets); g.bullets = n; }
    return 0;
}