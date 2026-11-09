#include "game.h"

#include "../ds/backtrack.h"
#include "../ds/bsp.h"
#include "../ds/dp.h"
#include "../ds/greedy.h"
#include "../ds/kmp.h"
#include "../ds/mst.h"
#include "../ds/path.h"
#include "../ds/search.h"
#include "../ds/sort.h"
#include "../ds/uf.h"

static const ItemDef CATALOG[] = {
    {1, "healing_potion", 8, 1, 10, 0, 1, 8},
    {2, "iron_sword", 12, 3, 0, 3, 2, 15},
    {3, "leather_armor", 10, 3, 0, 0, 2, 12},
    {4, "torch", 3, 1, 0, 0, 1, 3},
    {5, "antidote", 6, 1, 0, 0, 1, 6},
    {6, "firebomb", 9, 2, 0, 0, 2, 10},
};
static const int CATALOG_N = (int)(sizeof CATALOG / sizeof *CATALOG);

/* ---------- 日志 ---------- */

static void say(Game *g, const char *msg)
{
    char buf[96];
    snprintf(buf, sizeof buf, "%s", msg);
    kmp_filter_badwords(buf); /* 第 48：敏感词 */
    if (g->log_n < MAX_LOG) {
        snprintf(g->logs[g->log_n++], sizeof g->logs[0], "%s", buf);
    } else {
        memmove(g->logs[0], g->logs[1], (MAX_LOG - 1) * sizeof g->logs[0]);
        snprintf(g->logs[MAX_LOG - 1], sizeof g->logs[0], "%s", buf);
    }
}

/* ---------- 地图代价 / 通行 ---------- */

static bool in_map(int x, int y)
{
    return x >= 0 && x < MAP_W && y >= 0 && y < MAP_H;
}

static bool pass_tile(int x, int y, void *ctx)
{
    Game *g = ctx;
    if (!in_map(x, y)) {
        return false;
    }
    char t = g->map.tiles[y][x];
    return t != TILE_WALL;
}

static int tile_cost(int x, int y, void *ctx)
{
    Game *g = ctx;
    char t = g->map.tiles[y][x];
    if (t == TILE_MUD) {
        return 5;
    }
    if (t == TILE_TRAP) {
        return 20;
    }
    return 1;
}

static int ent_at(Game *g, int x, int y)
{
    int id;
    if (hashmap_get(&g->spatial, cell_key(x, y), &id)) {
        return id;
    }
    return -1;
}

static Entity *player(Game *g)
{
    return &g->ents[g->player_id];
}

/* ---------- 第 38+46+52+44：生成地牢 ---------- */

static void carve_room(DungeonMap *m, Point c)
{
    for (int y = c.y - 2; y <= c.y + 2; y++) {
        for (int x = c.x - 3; x <= c.x + 3; x++) {
            if (in_map(x, y)) {
                m->tiles[y][x] = TILE_FLOOR;
            }
        }
    }
}

static void carve_corridor(DungeonMap *m, Point a, Point b)
{
    int x = a.x, y = a.y;
    while (x != b.x) {
        if (in_map(x, y)) {
            m->tiles[y][x] = TILE_FLOOR;
        }
        x += x < b.x ? 1 : -1;
    }
    while (y != b.y) {
        if (in_map(x, y)) {
            m->tiles[y][x] = TILE_FLOOR;
        }
        y += y < b.y ? 1 : -1;
    }
    if (in_map(x, y)) {
        m->tiles[y][x] = TILE_FLOOR;
    }
}

static void generate_map(Game *g)
{
    DungeonMap *m = &g->map;
    memset(m->tiles, TILE_WALL, sizeof m->tiles);
    graph_free(&m->room_graph);

    BspNode *root = bsp_build(1, 1, MAP_W - 2, MAP_H - 2, 3);
    m->room_n = bsp_collect_rooms(root, m->rooms, MAX_ROOMS);
    bsp_free(root);
    if (m->room_n < 2) {
        m->room_n = 2;
        m->rooms[0] = (Point){6, 5};
        m->rooms[1] = (Point){MAP_W - 7, MAP_H - 6};
    }

    for (int i = 0; i < m->room_n; i++) {
        carve_room(m, m->rooms[i]);
    }

    MstEdge edges[64];
    int en = kruskal_rooms(m->rooms, m->room_n, edges, 64, &m->room_graph, 2);
    for (int i = 0; i < en; i++) {
        carve_corridor(m, m->rooms[edges[i].a], m->rooms[edges[i].b]);
    }

    /* 泥沼与陷阱 */
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (m->tiles[y][x] == TILE_FLOOR) {
                int r = rand() % 100;
                if (r < 6) {
                    m->tiles[y][x] = TILE_MUD;
                } else if (r < 9) {
                    m->tiles[y][x] = TILE_TRAP;
                }
            }
        }
    }

    /* 机关门放在最后一个房间 */
    Point door = m->rooms[m->room_n - 1];
    m->tiles[door.y][door.x] = TILE_DOOR;
    puzzle_derangement(4, g->puzzle);
    g->puzzle_open = false;

    if (!graph_connected_dfs(&m->room_graph)) {
        say(g, "警告：房间图未完全连通（教学检查）。");
    } else {
        say(g, "BSP+Kruskal 生成完成，房间已连通。");
    }
}

/* ---------- 实体 ---------- */

static int spawn(Game *g, EntKind kind, Point p, int hp, int atk, int speed, int faction, int item_id)
{
    if (g->ent_n >= MAX_ENTITIES) {
        return -1;
    }
    int i = g->ent_n++;
    g->ents[i] = (Entity){
        .kind = kind,
        .pos = p,
        .hp = hp,
        .max_hp = hp,
        .atk = atk,
        .speed = speed,
        .energy = 0,
        .faction = faction,
        .item_id = item_id,
        .alive = true,
    };
    hashmap_put(&g->spatial, cell_key(p.x, p.y), i);
    if (kind == ENT_MONSTER) {
        pool_alloc(&g->monster_pool, i);
    }
    heap_push(&g->turn_heap, i, speed);
    return i;
}

static void clear_entities(Game *g)
{
    g->ent_n = 0;
    pool_free(&g->monster_pool);
    pool_init(&g->monster_pool, 32);
    hashmap_free(&g->spatial);
    hashmap_init(&g->spatial, 97);
    heap_free(&g->turn_heap);
    heap_init(&g->turn_heap);
}

static void populate(Game *g)
{
    clear_entities(g);
    g->player_id = spawn(g, ENT_PLAYER, g->map.rooms[0], 30, 5, 10, 0, 0);
    player(g)->hp = 30;

    for (int i = 1; i < g->map.room_n; i++) {
        if (rand() % 100 < 70) {
            int fac = (i % 2) + 1; /* 两个敌对阵营，可合并 */
            spawn(g, ENT_MONSTER, g->map.rooms[i], 8 + g->depth * 2, 3 + g->depth, 12 + rand() % 5, fac, 0);
        }
        if (rand() % 100 < 40) {
            Point p = {g->map.rooms[i].x + 1, g->map.rooms[i].y};
            if (pass_tile(p.x, p.y, g)) {
                spawn(g, ENT_ITEM, p, 1, 0, 99, 0, 1 + rand() % CATALOG_N);
            }
        }
    }
}

/* ---------- 背包 ---------- */

static const ItemDef *item_def(Game *g, int id)
{
    int idx;
    if (!hashmap_get(&g->item_by_id, id, &idx)) {
        return NULL;
    }
    return &CATALOG[idx];
}

static int bag_weight(Game *g)
{
    int w = 0;
    for (int i = 0; i < g->bag_n; i++) {
        const ItemDef *d = item_def(g, g->bag[i].id);
        if (d) {
            w += d->weight * g->bag[i].count;
        }
    }
    return w;
}

static bool bag_add(Game *g, int id)
{
    const ItemDef *d = item_def(g, id);
    if (!d) {
        return false;
    }
    if (bag_weight(g) + d->weight > BAG_CAP_WEIGHT) {
        say(g, "负重不足，装不下。");
        return false;
    }
    for (int i = 0; i < g->bag_n; i++) {
        if (g->bag[i].id == id) {
            g->bag[i].count++;
            return true;
        }
    }
    if (g->bag_n >= 32) {
        return false;
    }
    g->bag[g->bag_n++] = (BagSlot){id, 1};
    return true;
}

static void show_bag(Game *g, int mode)
{
    BagRow rows[32];
    int n = 0;
    for (int i = 0; i < g->bag_n; i++) {
        const ItemDef *d = item_def(g, g->bag[i].id);
        if (!d) {
            continue;
        }
        rows[n] = (BagRow){d->id, d->price, d->rare, d->weight, n, d->name};
        n++;
    }
    if (mode == 1) {
        sort_by_price_mergesort(rows, (size_t)n);
        puts("-- 背包（归并按价格，稳定）--");
    } else if (mode == 2) {
        sort_by_rare(rows, (size_t)n);
        puts("-- 背包（稀有度）--");
    } else if (mode == 3) {
        sort_by_weight(rows, (size_t)n);
        puts("-- 背包（重量）--");
    } else {
        sort_by_price_qsort(rows, (size_t)n);
        puts("-- 背包（qsort 按价格）--");
    }
    printf("负重 %d/%d\n", bag_weight(g), BAG_CAP_WEIGHT);
    for (int i = 0; i < n; i++) {
        printf("%d) %-16s 价%d 稀有%d 重%d\n", i + 1, rows[i].name, rows[i].price, rows[i].rare,
               rows[i].weight);
    }
    if (n) {
        size_t lb = lower_bound_price(rows, (size_t)n, g->gold);
        printf("金币 %d；价格表 lower_bound -> 下标 %zu\n", g->gold, lb);
        int found = linear_find_id(rows, (size_t)n, 1);
        printf("线性查找 healing_potion id=1 -> %d\n", found);
    }
}

/* ---------- 悔棋 ---------- */

static void undo_push(Game *g)
{
    if (g->undo_top >= MAX_UNDO) {
        memmove(&g->undos[0], &g->undos[1], (MAX_UNDO - 1) * sizeof g->undos[0]);
        g->undo_top = MAX_UNDO - 1;
    }
    UndoSnap *s = &g->undos[g->undo_top++];
    Entity *p = player(g);
    *s = (UndoSnap){
        .px = p->pos.x,
        .py = p->pos.y,
        .gold = g->gold,
        .kills = g->kills,
        .depth = g->depth,
        .hp = p->hp,
        .bag_n = g->bag_n,
    };
    memcpy(s->bag, g->bag, sizeof g->bag);
}

static void undo_pop(Game *g)
{
    if (g->undo_top <= 0) {
        say(g, "没有可撤销的步骤。");
        return;
    }
    UndoSnap s = g->undos[--g->undo_top];
    Entity *p = player(g);
    hashmap_del(&g->spatial, cell_key(p->pos.x, p->pos.y));
    p->pos = (Point){s.px, s.py};
    p->hp = s.hp;
    g->gold = s.gold;
    g->kills = s.kills;
    g->depth = s.depth;
    g->bag_n = s.bag_n;
    memcpy(g->bag, s.bag, sizeof g->bag);
    hashmap_put(&g->spatial, cell_key(p->pos.x, p->pos.y), g->player_id);
    say(g, "已撤销一步（栈）。");
}

/* ---------- 战斗 / 移动 ---------- */

static void hit_monster(Game *g, int mi)
{
    Entity *m = &g->ents[mi];
    Entity *p = player(g);
    int bonus = 0;
    for (Buff *b = g->player_buffs; b; b = b->next) {
        bonus += b->atk_bonus;
    }
    m->hp -= p->atk + bonus;
    char msg[80];
    snprintf(msg, sizeof msg, "你造成 %d 伤害。", p->atk + bonus);
    say(g, msg);
    if (m->hp <= 0) {
        m->alive = false;
        hashmap_del(&g->spatial, cell_key(m->pos.x, m->pos.y));
        g->kills++;
        g->gold += 4;
        say(g, "怪物倒下，+4 金币。");
        /* 阵营合并演示：击杀后随机合并剩余怪物阵营 */
        UF u;
        uf_init(&u, 8);
        for (int i = 0; i < g->ent_n; i++) {
            if (g->ents[i].alive && g->ents[i].kind == ENT_MONSTER) {
                uf_union(&u, 1, g->ents[i].faction);
                g->ents[i].faction = uf_find(&u, g->ents[i].faction);
            }
        }
        uf_free(&u);
    } else {
        p->hp -= m->atk;
        say(g, "怪物反击！");
        if (rand() % 4 == 0) {
            buff_add(&g->player_buffs, "中毒", 3, 2, 0);
            say(g, "你中毒了。");
        }
        if (rand() % 5 == 0) {
            buff_add(&g->player_buffs, "燃烧", 2, 3, 0);
            say(g, "你被点燃了。");
        }
    }
}

static void try_move(Game *g, int dx, int dy)
{
    undo_push(g);
    Entity *p = player(g);
    int nx = p->pos.x + dx;
    int ny = p->pos.y + dy;
    if (!pass_tile(nx, ny, g)) {
        say(g, "撞墙了。");
        return;
    }
    if (g->map.tiles[ny][nx] == TILE_DOOR && !g->puzzle_open) {
        say(g, "机关门上锁。输入 p 解谜。");
        return;
    }
    int oid = ent_at(g, nx, ny);
    if (oid >= 0 && g->ents[oid].kind == ENT_MONSTER && g->ents[oid].alive) {
        hit_monster(g, oid);
        return;
    }
    if (oid >= 0 && g->ents[oid].kind == ENT_ITEM && g->ents[oid].alive) {
        if (bag_add(g, g->ents[oid].item_id)) {
            g->ents[oid].alive = false;
            hashmap_del(&g->spatial, cell_key(nx, ny));
            say(g, "捡起物品。");
        }
    }
    hashmap_del(&g->spatial, cell_key(p->pos.x, p->pos.y));
    p->pos = (Point){nx, ny};
    hashmap_put(&g->spatial, cell_key(nx, ny), g->player_id);
    if (g->map.tiles[ny][nx] == TILE_TRAP) {
        p->hp -= 4;
        say(g, "踩到陷阱！-4 HP");
    }
}

static void monster_chase(Game *g, int mi)
{
    Entity *m = &g->ents[mi];
    Entity *p = player(g);
    if (!m->alive) {
        return;
    }
    int prev[MAP_H][MAP_W];
    int dist;
    if (!dijkstra_to(m->pos, p->pos, pass_tile, tile_cost, g, prev, &dist)) {
        return;
    }
    Point path[8];
    int n = path_rebuild(m->pos, p->pos, prev, path, 8);
    if (n <= 0) {
        return;
    }
    Point step = path[0];
    if (step.x == p->pos.x && step.y == p->pos.y) {
        p->hp -= m->atk;
        say(g, "怪物追上来咬了你一口！");
        return;
    }
    if (ent_at(g, step.x, step.y) >= 0) {
        return;
    }
    hashmap_del(&g->spatial, cell_key(m->pos.x, m->pos.y));
    m->pos = step;
    hashmap_put(&g->spatial, cell_key(step.x, step.y), mi);
}

/* ---------- 商店 / 命令 ---------- */

static void print_shop_item(const ShopItem *it, void *ctx)
{
    (void)ctx;
    printf("  %s (%dG)\n", it->name, it->price);
}

static void shop(Game *g)
{
    BstNode *root = NULL;
    for (int i = 0; i < CATALOG_N; i++) {
        root = bst_insert(root, (ShopItem){CATALOG[i].id, CATALOG[i].price, CATALOG[i].name});
    }
    puts("\n-- 商店 BST 中序 --");
    bst_inorder(root, print_shop_item, NULL);
    const ShopItem *best = bst_floor_price(root, g->gold);
    if (best) {
        printf("预算内最贵：%s (%dG)\n", best->name, best->price);
    }
    for (int i = 0; i < CATALOG_N; i++) {
        printf("%d) %s %dG\n", i + 1, CATALOG[i].name, CATALOG[i].price);
    }
    printf("购买编号: ");
    char line[32];
    if (!fgets(line, sizeof line, stdin)) {
        bst_free(root);
        return;
    }
    int c = (int)strtol(line, NULL, 10);
    if (c >= 1 && c <= CATALOG_N && g->gold >= CATALOG[c - 1].price) {
        /* 找零演示 */
        int coins[] = {4, 3, 1};
        int change = 0; /* 刚好付清时找零 0；若多给演示用固定 6 */
        if (g->gold - CATALOG[c - 1].price >= 6) {
            change = 6;
        }
        g->gold -= CATALOG[c - 1].price;
        bag_add(g, CATALOG[c - 1].id);
        if (change) {
            int used = 0;
            greedy_change(change, coins, 3, &used);
            int opt = optimal_change_3_4_1(change);
            char msg[96];
            snprintf(msg, sizeof msg, "找零 %d：贪心 %d 枚，最优 %d 枚。", change, used, opt);
            say(g, msg);
            g->gold += change; /* 把演示找零退回口袋 */
        }
        say(g, "购买成功。");
    } else {
        say(g, "买不起或编号无效。");
    }
    bst_free(root);
}

static void auto_go(Game *g)
{
    Point goal = g->map.rooms[g->map.room_n - 1];
    int prev[MAP_H][MAP_W];
    int dist;
    if (!dijkstra_to(player(g)->pos, goal, pass_tile, tile_cost, g, prev, &dist)) {
        say(g, "自动前往：不可达。");
        return;
    }
    Point path[MAP_W * MAP_H];
    int n = path_rebuild(player(g)->pos, goal, prev, path, MAP_W * MAP_H);
    if (n <= 0) {
        return;
    }
    int dx = path[0].x - player(g)->pos.x;
    int dy = path[0].y - player(g)->pos.y;
    try_move(g, dx, dy);
}

static void solve_puzzle(Game *g)
{
    puts("机关门密码锁：输入 4 个数字（错位排列答案），空格分隔");
    printf("提示：系统已生成一组错位排列。演示答案：");
    for (int i = 0; i < 4; i++) {
        printf("%d ", g->puzzle[i]);
    }
    puts("");
    int guess[4];
    if (scanf("%d %d %d %d", &guess[0], &guess[1], &guess[2], &guess[3]) != 4) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
        say(g, "输入无效。");
        return;
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
    if (puzzle_check_perm(g->puzzle, guess, 4)) {
        g->puzzle_open = true;
        Point d = g->map.rooms[g->map.room_n - 1];
        g->map.tiles[d.y][d.x] = TILE_FLOOR;
        say(g, "机关门打开了！");
    } else {
        say(g, "密码错误。");
    }
}

static void suggest_loadout(Game *g)
{
    int w[16], v[16], n = 0;
    for (int i = 0; i < g->bag_n && n < 16; i++) {
        const ItemDef *d = item_def(g, g->bag[i].id);
        if (!d) {
            continue;
        }
        w[n] = d->weight;
        v[n] = d->value;
        n++;
    }
    int best = knapsack01(w, v, n, BAG_CAP_WEIGHT);
    int skill[] = {1, 3, 2, 5, 4, 7};
    printf("0-1 背包建议最大价值=%d；技能线 LIS=%d；heal→healing 编辑距离=%d\n", best,
           lis_length(skill, 6), edit_distance("heal", "healing"));
}

static void print_score(const Score *s, void *ctx)
{
    int *rank = ctx;
    printf("%d) %s 总分%d (金币%d 击杀%d 深度%d)\n", *rank, s->name, s->total, s->gold, s->kills,
           s->depth);
    (*rank)++;
}

static void record_score(Game *g)
{
    Score s = {
        .total = g->kills * 100 + g->gold + g->depth * 50,
        .gold = g->gold,
        .kills = g->kills,
        .depth = g->depth,
    };
    snprintf(s.name, sizeof s.name, "run%d", avl_count(g->board) + 1);
    g->board = avl_insert(g->board, s);
    while (avl_count(g->board) > 8) {
        g->board = avl_remove_min(g->board);
    }
}

static void show_board(Game *g)
{
    puts("\n-- AVL 排行榜（多维键：总分/击杀/金币/深度）--");
    int rank = 1;
    avl_each_desc(g->board, print_score, &rank);
}

static void draw(Game *g)
{
    Entity *p = player(g);
    printf("\n层%d HP:%d/%d 金%d 杀%d 负重%d/%d\n", g->depth, p->hp, p->max_hp, g->gold, g->kills,
           bag_weight(g), BAG_CAP_WEIGHT);
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            char c = g->map.tiles[y][x];
            int id = ent_at(g, x, y);
            if (id == g->player_id) {
                c = '@';
            } else if (id >= 0 && g->ents[id].kind == ENT_MONSTER && g->ents[id].alive) {
                c = 'g';
            } else if (id >= 0 && g->ents[id].kind == ENT_ITEM && g->ents[id].alive) {
                c = '*';
            }
            putchar(c);
        }
        putchar('\n');
    }
    int from = g->log_n > 4 ? g->log_n - 4 : 0;
    for (int i = from; i < g->log_n; i++) {
        printf("· %s\n", g->logs[i]);
    }
    int vision = bfs_count(p->pos, pass_tile, g, 6);
    printf("视野连通格(BFS≤6): %d\n", vision);
}

static void help(void)
{
    puts("wasd移动 | b背包 | 1/2/3切换排序 | u用药 | t商店 | h自动前往 | p解谜");
    puts("z悔棋 | l搜日志 | c补全 | ?建议/演示 | n下层 | r排行 | q退出");
}

static void process_events(Game *g)
{
    int ev;
    while (queue_pop(&g->event_q, &ev)) {
        if (ev == 1) {
            say(g, "事件队列：远处传来怪叫。");
        } else if (ev == 2) {
            say(g, "事件队列：你感到一丝寒意。");
        }
    }
}

static void tick_turns(Game *g)
{
    /* 简化：玩家指令后，所有怪物各追一步；堆用于决定怪物行动次序 */
    MinHeap order;
    heap_init(&order);
    for (int i = 0; i < g->ent_n; i++) {
        if (g->ents[i].alive && g->ents[i].kind == ENT_MONSTER) {
            heap_push(&order, i, g->ents[i].speed + rand() % 3);
        }
    }
    while (!heap_empty(&order)) {
        HeapItem it;
        heap_pop(&order, &it);
        monster_chase(g, it.id);
    }
    heap_free(&order);

    int dmg = buff_tick(&g->player_buffs);
    if (dmg) {
        player(g)->hp -= dmg;
        char msg[64];
        snprintf(msg, sizeof msg, "持续效果伤害 %d。", dmg);
        say(g, msg);
    }
    if (rand() % 5 == 0) {
        queue_push(&g->event_q, 1 + rand() % 2);
    }
    process_events(g);
}

static void next_floor(Game *g)
{
    record_score(g);
    g->depth++;
    player(g)->hp = player(g)->max_hp;
    generate_map(g);
    populate(g);
    say(g, "进入更深的一层。");
}

void game_init(Game *g)
{
    memset(g, 0, sizeof *g);
    g->running = true;
    g->depth = 1;
    g->gold = 20;
    hashmap_init(&g->item_by_id, 17);
    for (int i = 0; i < CATALOG_N; i++) {
        hashmap_put(&g->item_by_id, CATALOG[i].id, i);
    }
    hashmap_init(&g->spatial, 97);
    pool_init(&g->monster_pool, 32);
    queue_init(&g->event_q, 64);
    heap_init(&g->turn_heap);
    trie_init(&g->cmds);
    const char *cmds[] = {"help", "bag", "shop", "heal", "healing_potion", "go", "quit", "search",
                          "hint", "map"};
    for (size_t i = 0; i < sizeof cmds / sizeof *cmds; i++) {
        trie_add(&g->cmds, cmds[i]);
    }
    generate_map(g);
    populate(g);
    bag_add(g, 1);
    say(g, "欢迎来到地牢。输入 ? 看算法建议，c 试 Trie 补全。");
}

void game_free(Game *g)
{
    graph_free(&g->map.room_graph);
    hashmap_free(&g->item_by_id);
    hashmap_free(&g->spatial);
    pool_free(&g->monster_pool);
    queue_free(&g->event_q);
    heap_free(&g->turn_heap);
    buff_clear(&g->player_buffs);
    trie_free(&g->cmds);
    avl_free(g->board);
}

void game_loop(Game *g)
{
    char line[160];
    while (g->running && player(g)->hp > 0) {
        draw(g);
        printf("> ");
        if (!fgets(line, sizeof line, stdin)) {
            break;
        }
        line[strcspn(line, "\r\n")] = 0;
        if (!line[0]) {
            continue;
        }

        /* Trie 补全：输入前缀后跟 '?' */
        size_t L = strlen(line);
        if (L >= 2 && line[L - 1] == '?') {
            line[L - 1] = 0;
            char outs[16][32];
            int n = trie_complete(&g->cmds, line, outs, 16);
            printf("补全:");
            for (int i = 0; i < n; i++) {
                printf(" %s", outs[i]);
            }
            puts(n ? "" : " （无）");
            continue;
        }

        char c = (char)tolower((unsigned char)line[0]);
        switch (c) {
        case 'w':
            try_move(g, 0, -1);
            tick_turns(g);
            break;
        case 's':
            try_move(g, 0, 1);
            tick_turns(g);
            break;
        case 'a':
            try_move(g, -1, 0);
            tick_turns(g);
            break;
        case 'd':
            try_move(g, 1, 0);
            tick_turns(g);
            break;
        case 'b':
            show_bag(g, 0);
            break;
        case '1':
            show_bag(g, 1);
            break;
        case '2':
            show_bag(g, 2);
            break;
        case '3':
            show_bag(g, 3);
            break;
        case 'u': {
            for (int i = 0; i < g->bag_n; i++) {
                const ItemDef *d = item_def(g, g->bag[i].id);
                if (d && d->heal) {
                    player(g)->hp += d->heal;
                    if (player(g)->hp > player(g)->max_hp) {
                        player(g)->hp = player(g)->max_hp;
                    }
                    if (--g->bag[i].count <= 0) {
                        memmove(&g->bag[i], &g->bag[i + 1], (size_t)(g->bag_n - i - 1) * sizeof *g->bag);
                        g->bag_n--;
                    }
                    say(g, "喝下治疗药水。");
                    break;
                }
            }
            break;
        }
        case 't':
            shop(g);
            break;
        case 'h':
            auto_go(g);
            tick_turns(g);
            break;
        case 'z':
            undo_pop(g);
            break;
        case 'p':
            solve_puzzle(g);
            break;
        case 'l': {
            char key[64];
            printf("日志关键字: ");
            if (fgets(key, sizeof key, stdin)) {
                key[strcspn(key, "\r\n")] = 0;
                int hits = 0;
                for (int i = 0; i < g->log_n; i++) {
                    if (kmp_contains(g->logs[i], key)) {
                        printf("命中: %s\n", g->logs[i]);
                        hits++;
                    }
                }
                if (!hits) {
                    puts("无匹配");
                }
            }
            break;
        }
        case 'c': {
            char outs[16][32];
            int n = trie_complete(&g->cmds, "he", outs, 16);
            printf("he 补全:");
            for (int i = 0; i < n; i++) {
                printf(" %s", outs[i]);
            }
            puts("");
            break;
        }
        case '?':
            suggest_loadout(g);
            help();
            break;
        case 'n':
            next_floor(g);
            break;
        case 'r':
            record_score(g);
            show_board(g);
            break;
        case 'q':
            g->running = false;
            break;
        default:
            help();
            break;
        }
    }
    record_score(g);
    puts(player(g)->hp > 0 ? "你离开了地牢。" : "你倒下了……");
    show_board(g);
}
