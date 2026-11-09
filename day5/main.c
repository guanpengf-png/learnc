/*
 * 地牢探险：C 语言数据结构与算法学习游戏
 * 编译：make；运行：./dungeon
 *
 * 单文件教学版。函数旁标注“第 NN 章”，对应 32–52 章知识点。
 * 随机地图、回合战斗、背包、商店、命令补全、日志检索和排行榜均可试玩。
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define W 40
#define H 18
#define MAX_MONSTERS 64
#define MAX_ITEMS 32
#define LOG_CAP 80
#define INF 1000000

typedef struct {
    int x, y;
} Point;

typedef struct {
    int x, y, hp, attack, alive;
} Monster;

typedef struct {
    int id, price, weight, heal;
    const char *name;
} ItemDef;

typedef struct {
    int id, count;
} InventoryItem;

/* 第 33 章：动态数组 */
typedef struct {
    InventoryItem *data;
    size_t len, cap;
} Vector;

/* 第 34 章：链表结点 */
typedef struct Effect {
    char name[20];
    int turns, damage;
    struct Effect *next;
} Effect;

/* 第 32、37 章：哈希桶里的键值对 */
typedef struct Entry {
    int key;
    const ItemDef *value;
    struct Entry *next;
} Entry;

static Entry *item_table[17];
static const ItemDef catalog[] = {
    {1, 8, 1, 8, "healing_potion"},
    {2, 5, 2, 0, "iron_sword"},
    {3, 12, 3, 0, "leather_armor"},
    {4, 3, 1, 0, "torch"},
};
static const size_t catalog_n = sizeof catalog / sizeof catalog[0];

static char map[H][W];
static Monster monsters[MAX_MONSTERS];
static size_t monster_n;
static Vector bag;
static Effect *effects;
static char logs[LOG_CAP][120];
static size_t log_head, log_len;
static Point player;
static int hp = 30, gold = 18, depth = 1, kills;

static int read_choice(const char *prompt);

/* ---------- 日志（循环缓冲区） ---------- */

static void say(const char *s)
{
    /* 第 33 章思路：固定容量。满了就覆盖最旧消息。 */
    size_t i = (log_head + log_len) % LOG_CAP;
    if (log_len == LOG_CAP) {
        log_head = (log_head + 1) % LOG_CAP;
        i = (log_head + log_len - 1) % LOG_CAP;
    } else {
        log_len++;
    }
    snprintf(logs[i], sizeof logs[i], "%s", s);
}

static void show_logs(void)
{
    puts("\n-- 最近消息 --");
    for (size_t i = 0; i < log_len; i++) {
        puts(logs[(log_head + i) % LOG_CAP]);
    }
}

/* ---------- 第 37 章：哈希表 ---------- */

static size_t hash_id(int id)
{
    return (size_t)(unsigned)id % 17u;
}

static void item_index_init(void)
{
    for (size_t i = 0; i < catalog_n; i++) {
        size_t b = hash_id(catalog[i].id);
        Entry *e = malloc(sizeof *e);
        if (!e) {
            exit(1);
        }
        e->key = catalog[i].id;
        e->value = &catalog[i];
        e->next = item_table[b];
        item_table[b] = e;
    }
}

static const ItemDef *item_get(int id)
{
    for (Entry *e = item_table[hash_id(id)]; e; e = e->next) {
        if (e->key == id) {
            return e->value;
        }
    }
    return NULL;
}

/* ---------- 第 33 章：动态数组背包 ---------- */

static bool bag_add(int id)
{
    for (size_t i = 0; i < bag.len; i++) {
        if (bag.data[i].id == id) {
            bag.data[i].count++;
            return true;
        }
    }
    if (bag.len == bag.cap) {
        size_t cap = bag.cap ? bag.cap * 2 : 4;
        InventoryItem *p = realloc(bag.data, cap * sizeof *p);
        if (!p) {
            return false;
        }
        bag.data = p;
        bag.cap = cap;
    }
    bag.data[bag.len++] = (InventoryItem){id, 1};
    return true;
}

static bool bag_remove(size_t i)
{
    if (i >= bag.len) {
        return false;
    }
    if (--bag.data[i].count == 0) {
        memmove(&bag.data[i], &bag.data[i + 1], (bag.len - i - 1) * sizeof *bag.data);
        bag.len--;
    }
    return true;
}

static void show_bag(void)
{
    puts("\n-- 背包 --");
    if (!bag.len) {
        puts("（空）");
    }
    for (size_t i = 0; i < bag.len; i++) {
        const ItemDef *d = item_get(bag.data[i].id);
        printf("%zu) %-18s x%d 价格:%d 重量:%d\n",
               i + 1,
               d ? d->name : "unknown",
               bag.data[i].count,
               d ? d->price : 0,
               d ? d->weight : 0);
    }
}

/* ---------- 第 34 章：效果链表 ---------- */

static void add_poison(void)
{
    Effect *e = malloc(sizeof *e);
    if (!e) {
        return;
    }
    strcpy(e->name, "中毒");
    e->turns = 3;
    e->damage = 2;
    e->next = effects;
    effects = e;
}

static void tick_effects(void)
{
    /* 二级指针：删头结点和删中间结点同一写法。 */
    Effect **pp = &effects;
    while (*pp) {
        Effect *e = *pp;
        hp -= e->damage;
        char msg[80];
        snprintf(msg, sizeof msg, "%s造成 %d 点伤害。", e->name, e->damage);
        say(msg);
        if (--e->turns <= 0) {
            *pp = e->next;
            free(e);
        } else {
            pp = &e->next;
        }
    }
}

/* ---------- 第 52 章：并查集 ---------- */

typedef struct {
    int p[8], rank[8];
} UF;

static int uf_find(UF *u, int x)
{
    return u->p[x] == x ? x : (u->p[x] = uf_find(u, u->p[x]));
}

static bool uf_union(UF *u, int a, int b)
{
    a = uf_find(u, a);
    b = uf_find(u, b);
    if (a == b) {
        return false;
    }
    if (u->rank[a] < u->rank[b]) {
        int t = a;
        a = b;
        b = t;
    }
    u->p[b] = a;
    if (u->rank[a] == u->rank[b]) {
        u->rank[a]++;
    }
    return true;
}

/* ---------- 第 46 章：Kruskal 连通房间 ---------- */

typedef struct {
    int a, b, w;
} Edge;

static void carve(int x, int y)
{
    if (x >= 0 && x < W && y >= 0 && y < H) {
        map[y][x] = '.';
    }
}

static void corridor(Point a, Point b)
{
    int x = a.x, y = a.y;
    while (x != b.x) {
        carve(x, y);
        x += x < b.x ? 1 : -1;
    }
    while (y != b.y) {
        carve(x, y);
        y += y < b.y ? 1 : -1;
    }
    carve(x, y);
}

static void make_dungeon(void)
{
    memset(map, '#', sizeof map);
    Point rooms[8];
    for (int i = 0; i < 8; i++) {
        rooms[i] = (Point){3 + (i % 4) * 10, 3 + (i / 4) * 9};
        for (int y = rooms[i].y - 2; y <= rooms[i].y + 2; y++) {
            for (int x = rooms[i].x - 3; x <= rooms[i].x + 3; x++) {
                carve(x, y);
            }
        }
    }

    Edge edges[28];
    int n = 0;
    for (int a = 0; a < 8; a++) {
        for (int b = a + 1; b < 8; b++) {
            edges[n++] = (Edge){a, b, rand() % 100};
        }
    }
    /* 教学版随机打乱边序，不保证走廊总长最短。 */
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Edge t = edges[i];
        edges[i] = edges[j];
        edges[j] = t;
    }

    UF u;
    for (int i = 0; i < 8; i++) {
        u.p[i] = i;
        u.rank[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        if (uf_union(&u, edges[i].a, edges[i].b)) {
            corridor(rooms[edges[i].a], rooms[edges[i].b]);
        }
    }

    /* 泥沼给 Dijkstra 准备非均匀边权。 */
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (map[y][x] == '.' && rand() % 100 < 7) {
                map[y][x] = '~';
            }
        }
    }

    /* 第 35 章：DFS 栈验证房间连通。 */
    bool seen[8] = {0};
    int stack[8], top = 0;
    stack[top++] = 0;
    seen[0] = true;
    while (top) {
        int v = stack[--top];
        for (int j = 0; j < n; j++) {
            int w = edges[j].a == v ? edges[j].b
                  : edges[j].b == v ? edges[j].a
                  : -1;
            if (w >= 0 && !seen[w]) {
                seen[w] = true;
                stack[top++] = w;
            }
        }
    }

    player = rooms[0];
    monster_n = 0;
    for (int i = 1; i < 8; i++) {
        if (rand() % 100 < 55 && monster_n < MAX_MONSTERS) {
            monsters[monster_n++] = (Monster){
                rooms[i].x, rooms[i].y, 8 + depth * 2, 3 + depth, 1};
        }
    }
    if (!bag.len) {
        bag_add(1);
    }
    say("你踏入了一座新的地牢。");
}

static bool passable(int x, int y)
{
    return x >= 0 && x < W && y >= 0 && y < H && map[y][x] != '#';
}

static int monster_at(int x, int y)
{
    for (size_t i = 0; i < monster_n; i++) {
        if (monsters[i].alive && monsters[i].x == x && monsters[i].y == y) {
            return (int)i;
        }
    }
    return -1;
}

/* ---------- 第 41、45 章：最小堆 + Dijkstra ---------- */

typedef struct {
    int id, dist;
} HeapNode;

static HeapNode heap[W * H * 4];
static int hn;

static void hp_swap(int a, int b)
{
    HeapNode t = heap[a];
    heap[a] = heap[b];
    heap[b] = t;
}

static void push_heap(HeapNode n)
{
    int i = hn++;
    heap[i] = n;
    while (i) {
        int p = (i - 1) / 2;
        if (heap[p].dist <= heap[i].dist) {
            break;
        }
        hp_swap(p, i);
        i = p;
    }
}

static HeapNode pop_heap(void)
{
    HeapNode r = heap[0];
    heap[0] = heap[--hn];
    int i = 0;
    for (;;) {
        int l = i * 2 + 1, rn = l + 1, m = i;
        if (l < hn && heap[l].dist < heap[m].dist) {
            m = l;
        }
        if (rn < hn && heap[rn].dist < heap[m].dist) {
            m = rn;
        }
        if (m == i) {
            break;
        }
        hp_swap(i, m);
        i = m;
    }
    return r;
}

static void dijkstra(Point goal, int prev[H][W])
{
    int d[H][W];
    bool done[H][W] = {0};
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            d[y][x] = INF;
            prev[y][x] = -1;
        }
    }
    hn = 0;
    d[player.y][player.x] = 0;
    push_heap((HeapNode){player.y * W + player.x, 0});

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    while (hn) {
        HeapNode z = pop_heap();
        int x = z.id % W;
        int y = z.id / W;
        if (done[y][x]) {
            continue;
        }
        done[y][x] = true;
        if (x == goal.x && y == goal.y) {
            break;
        }
        for (int k = 0; k < 4; k++) {
            int a = x + dx[k];
            int b = y + dy[k];
            int yy = b < 0 ? 0 : (b >= H ? H - 1 : b);
            int xx = a < 0 ? 0 : (a >= W ? W - 1 : a);
            int cost = map[yy][xx] == '~' ? 5 : 1;
            if (passable(a, b) && d[y][x] + cost < d[b][a]) {
                d[b][a] = d[y][x] + cost;
                prev[b][a] = y * W + x;
                push_heap((HeapNode){b * W + a, d[b][a]});
            }
        }
    }
}

/* ---------- 第 39、42、43 章：BST 商店 + 排序 + 二分 ---------- */

static int by_price(const void *a, const void *b)
{
    const ItemDef *x = *(const ItemDef **)a;
    const ItemDef *y = *(const ItemDef **)b;
    return (x->price > y->price) - (x->price < y->price);
}

static size_t lower_price(const ItemDef **a, size_t n, int budget)
{
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if (a[m]->price <= budget) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    return l;
}

typedef struct BNode {
    const ItemDef *d;
    struct BNode *l, *r;
} BNode;

static BNode *bst_add(BNode *r, const ItemDef *d)
{
    if (!r) {
        BNode *n = calloc(1, sizeof *n);
        if (n) {
            n->d = d;
        }
        return n;
    }
    if (d->price < r->d->price) {
        r->l = bst_add(r->l, d);
    } else {
        r->r = bst_add(r->r, d);
    }
    return r;
}

static void bst_print(BNode *r)
{
    if (!r) {
        return;
    }
    bst_print(r->l);
    printf("%s (%dG) ", r->d->name, r->d->price);
    bst_print(r->r);
}

static void bst_free(BNode *r)
{
    if (!r) {
        return;
    }
    bst_free(r->l);
    bst_free(r->r);
    free(r);
}

static void shop(void)
{
    const ItemDef *sorted[8];
    BNode *root = NULL;
    for (size_t i = 0; i < catalog_n; i++) {
        sorted[i] = &catalog[i];
        root = bst_add(root, &catalog[i]);
    }
    qsort(sorted, catalog_n, sizeof *sorted, by_price);

    puts("\n商店（BST 中序）:");
    bst_print(root);
    puts("");

    size_t affordable = lower_price(sorted, catalog_n, gold);
    printf("你有 %dG；买得起 %zu 件不同商品中价格最高的一件：", gold, affordable);
    if (affordable) {
        puts(sorted[affordable - 1]->name);
    } else {
        puts("无");
    }

    for (size_t i = 0; i < catalog_n; i++) {
        printf("%zu) %s - %dG\n", i + 1, catalog[i].name, catalog[i].price);
    }
    int c = read_choice("购买编号 (0取消): ");
    if (c > 0 && (size_t)c <= catalog_n && gold >= catalog[c - 1].price) {
        gold -= catalog[c - 1].price;
        bag_add(catalog[c - 1].id);
        say("你买下一件物品。");
    } else if (c) {
        puts("金币不足或编号无效。");
    }
    bst_free(root);
}

/* ---------- 第 40 章：AVL 排行榜 ---------- */

typedef struct Avl {
    int score, height;
    struct Avl *l, *r;
} Avl;

static int ah(Avl *n)
{
    return n ? n->height : 0;
}

static int imax(int a, int b)
{
    return a > b ? a : b;
}

static void upd(Avl *n)
{
    n->height = 1 + imax(ah(n->l), ah(n->r));
}

static Avl *rot_r(Avl *y)
{
    Avl *x = y->l;
    Avl *t = x->r;
    x->r = y;
    y->l = t;
    upd(y);
    upd(x);
    return x;
}

static Avl *rot_l(Avl *x)
{
    Avl *y = x->r;
    Avl *t = y->l;
    y->l = x;
    x->r = t;
    upd(x);
    upd(y);
    return y;
}

static Avl *avl_add(Avl *n, int s)
{
    if (!n) {
        Avl *x = calloc(1, sizeof *x);
        if (x) {
            x->score = s;
            x->height = 1;
        }
        return x;
    }
    if (s < n->score) {
        n->l = avl_add(n->l, s);
    } else {
        n->r = avl_add(n->r, s);
    }
    upd(n);
    int b = ah(n->l) - ah(n->r);
    if (b > 1 && s < n->l->score) {
        return rot_r(n);
    }
    if (b < -1 && s >= n->r->score) {
        return rot_l(n);
    }
    if (b > 1) {
        n->l = rot_l(n->l);
        return rot_r(n);
    }
    if (b < -1) {
        n->r = rot_r(n->r);
        return rot_l(n);
    }
    return n;
}

static void avl_desc(Avl *n)
{
    if (!n) {
        return;
    }
    avl_desc(n->r);
    printf("%d\n", n->score);
    avl_desc(n->l);
}

static Avl *score_root;

static int current_score(void)
{
    return kills * 20 + gold + depth * 10;
}

static void leaderboard_record(void)
{
    score_root = avl_add(score_root, current_score());
}

static void leaderboard(void)
{
    leaderboard_record();
    puts("\n本次分数（击杀×20 + 金币 + 深度×10）及本局各层排行榜：");
    avl_desc(score_root);
}

/* ---------- 第 47 章：动态规划 ---------- */

static int best_loadout(void)
{
    int w[] = {2, 3, 4, 5};
    int v[] = {3, 6, 7, 9};
    int dp[11] = {0};
    for (int i = 0; i < 4; i++) {
        for (int c = 10; c >= w[i]; c--) {
            if (dp[c - w[i]] + v[i] > dp[c]) {
                dp[c] = dp[c - w[i]] + v[i];
            }
        }
    }
    return dp[10];
}

static int lis_demo(void)
{
    int a[] = {1, 3, 2, 5, 4};
    int d[5], best = 0;
    for (int i = 0; i < 5; i++) {
        d[i] = 1;
        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && d[j] + 1 > d[i]) {
                d[i] = d[j] + 1;
            }
        }
        if (d[i] > best) {
            best = d[i];
        }
    }
    return best;
}

static int edit_distance(const char *a, const char *b)
{
    int d[32][32];
    int n = (int)strlen(a);
    int m = (int)strlen(b);
    for (int i = 0; i <= n; i++) {
        d[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        d[0][j] = j;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int c = a[i - 1] != b[j - 1];
            int x = d[i - 1][j] + 1;
            int y = d[i][j - 1] + 1;
            int z = d[i - 1][j - 1] + c;
            d[i][j] = x < y ? (x < z ? x : z) : (y < z ? y : z);
        }
    }
    return d[n][m];
}

/* ---------- 第 48 章：KMP ---------- */

static void log_search(const char *p)
{
    int m = (int)strlen(p);
    int pi[120] = {0};
    int k = 0;
    if (!m) {
        return;
    }
    for (int i = 1; i < m; i++) {
        while (k && p[i] != p[k]) {
            k = pi[k - 1];
        }
        if (p[i] == p[k]) {
            k++;
        }
        pi[i] = k;
    }

    int hits = 0;
    for (size_t r = 0; r < log_len; r++) {
        const char *t = logs[(log_head + r) % LOG_CAP];
        k = 0;
        for (int i = 0; t[i]; i++) {
            while (k && t[i] != p[k]) {
                k = pi[k - 1];
            }
            if (t[i] == p[k]) {
                k++;
            }
            if (k == m) {
                printf("找到：%s\n", t);
                hits++;
                k = pi[k - 1];
            }
        }
    }
    if (!hits) {
        puts("没有匹配的日志。");
    }
}

/* ---------- 第 49 章：Trie ---------- */

typedef struct Trie {
    struct Trie *c[26];
    bool end;
} Trie;

static Trie trie;

static void trie_add(const char *s)
{
    Trie *n = &trie;
    for (; *s; s++) {
        if (!islower((unsigned char)*s)) {
            return;
        }
        int i = *s - 'a';
        if (!n->c[i]) {
            n->c[i] = calloc(1, sizeof(Trie));
        }
        n = n->c[i];
    }
    n->end = true;
}

static bool trie_has(const char *s)
{
    Trie *n = &trie;
    for (; *s; s++) {
        int i = *s - 'a';
        if (i < 0 || i >= 26 || !n->c[i]) {
            return false;
        }
        n = n->c[i];
    }
    return n->end;
}

static void complete(const char *s)
{
    char prefix[40];
    snprintf(prefix, sizeof prefix, "%s", s);
    Trie *n = &trie;
    for (const char *p = prefix; *p; p++) {
        int i = *p - 'a';
        if (i < 0 || i >= 26 || !n->c[i]) {
            puts("没有匹配命令");
            return;
        }
        n = n->c[i];
    }
    const char *cmd[] = {
        "help", "map", "bag", "shop", "log", "search", "hint", "go", "quit"};
    for (size_t i = 0; i < sizeof cmd / sizeof *cmd; i++) {
        if (strncmp(cmd[i], prefix, strlen(prefix)) == 0) {
            printf("%s%s", cmd[i], trie_has(cmd[i]) ? " (命令)\n" : "\n");
        }
    }
}

/* ---------- 第 50 章：回溯 ---------- */

static bool route_dfs(int x, int y, bool seen[H][W], Point *out, int *len, int limit)
{
    if (*len >= limit) {
        return false;
    }
    if (x == 33 && y == 12) {
        return true;
    }
    seen[y][x] = true;
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    for (int k = 0; k < 4; k++) {
        int a = x + dx[k];
        int b = y + dy[k];
        if (passable(a, b) && !seen[b][a]) {
            out[(*len)++] = (Point){a, b};
            if (route_dfs(a, b, seen, out, len, limit)) {
                return true;
            }
            (*len)--;
        }
    }
    seen[y][x] = false;
    return false;
}

/* ---------- 第 51 章：贪心找零反例 ---------- */

static void greedy_demo(void)
{
    int coins[] = {4, 3, 1};
    int left = 6;
    int n = 0;
    for (int i = 0; i < 3; i++) {
        while (left >= coins[i]) {
            left -= coins[i];
            n++;
        }
    }
    printf("贪心找 6 用 %d 枚；但 3+3 只需 2 枚（所以贪心不总最优）。\n", n);
}

/* ---------- 第 36、44 章：BFS ---------- */

static int bfs_reachable(void)
{
    Point q[W * H];
    bool seen[H][W] = {0};
    int front = 0, rear = 0, count = 0;
    q[rear++] = player;
    seen[player.y][player.x] = true;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    while (front < rear) {
        Point p = q[front++];
        count++;
        for (int k = 0; k < 4; k++) {
            int x = p.x + dx[k];
            int y = p.y + dy[k];
            if (passable(x, y) && !seen[y][x]) {
                seen[y][x] = true;
                q[rear++] = (Point){x, y};
            }
        }
    }
    return count;
}

/* ---------- 交互与战斗 ---------- */

static int read_choice(const char *prompt)
{
    char line[128];
    printf("%s", prompt);
    if (!fgets(line, sizeof line, stdin)) {
        return -1;
    }
    return (int)strtol(line, NULL, 10);
}

static void draw(void)
{
    printf("\n地牢 %d层  HP:%d/30  金币:%d  击杀:%d\n", depth, hp, gold, kills);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            char c = map[y][x];
            if (x == player.x && y == player.y) {
                c = '@';
            } else if (monster_at(x, y) >= 0) {
                c = 'g';
            }
            putchar(c);
        }
        putchar('\n');
    }
    for (size_t i = log_len > 4 ? log_len - 4 : 0; i < log_len; i++) {
        printf("· %s\n", logs[(log_head + i) % LOG_CAP]);
    }
}

static void inventory_use(void)
{
    show_bag();
    int c = read_choice("使用哪件物品 (0取消): ");
    if (c > 0 && (size_t)c <= bag.len) {
        const ItemDef *d = item_get(bag.data[c - 1].id);
        if (d && d->heal) {
            hp += d->heal;
            if (hp > 30) {
                hp = 30;
            }
            bag_remove((size_t)c - 1);
            say("药水恢复了生命。");
        }
    }
}

static void fight_or_move(int dx, int dy)
{
    int x = player.x + dx;
    int y = player.y + dy;
    int mi = monster_at(x, y);
    if (mi >= 0) {
        monsters[mi].hp -= 5;
        if (monsters[mi].hp <= 0) {
            monsters[mi].alive = 0;
            kills++;
            gold += 4;
            say("怪物倒下了，你得到 4 金币。");
        } else {
            hp -= monsters[mi].attack;
            say("你击中怪物，怪物反击！");
            if (rand() % 5 == 0) {
                add_poison();
            }
        }
        return;
    }
    if (passable(x, y)) {
        player = (Point){x, y};
    } else {
        say("前方是墙。");
    }
}

static void help(void)
{
    puts("\n命令：w/a/s/d 移动或攻击 | b 背包 | u 使用物品 | h 自动寻路 | "
         "t 商店 | l 搜索日志 | ? 算法演示 | n 下一层 | q 退出");
}

static void algorithm_menu(void)
{
    printf("\n0-1背包最优价值(容量10): %d；技能升级 LIS 长度: %d\n",
           best_loadout(), lis_demo());
    printf("BFS 可达地格数: %d；拼写距离 'heal'→'healing': %d\n",
           bfs_reachable(), edit_distance("heal", "healing"));
    greedy_demo();

    Point route[W * H];
    int len = 0;
    bool seen[H][W] = {0};
    if (route_dfs(player.x, player.y, seen, route, &len, W * H)) {
        printf("回溯找到地图深处路线，长度 %d 格。\n", len);
    } else {
        puts("回溯没有找到目标路线。");
    }
}

static void auto_go(void)
{
    Point goal = {33, 12};
    int prev[H][W];
    dijkstra(goal, prev);
    int id = prev[goal.y][goal.x];
    if (id < 0) {
        puts("目标不可达");
        return;
    }
    int x = id % W;
    int y = id / W;
    if (monster_at(x, y) >= 0) {
        fight_or_move(x - player.x, y - player.y);
        return;
    }
    player = (Point){x, y};
}

/* ---------- 主循环 ---------- */

int main(void)
{
    srand((unsigned)time(NULL));
    item_index_init();

    const char *cmds[] = {
        "help", "map", "bag", "shop", "log", "search", "hint", "go", "quit"};
    for (size_t i = 0; i < sizeof cmds / sizeof *cmds; i++) {
        trie_add(cmds[i]);
    }

    bag_add(1);
    make_dungeon();
    say("按 ? 查看学习算法演示。");

    char line[160];
    bool running = true;
    while (running && hp > 0) {
        draw();
        printf("\n> ");
        if (!fgets(line, sizeof line, stdin)) {
            break;
        }
        char c = (char)tolower((unsigned char)line[0]);
        switch (c) {
        case 'w':
            fight_or_move(0, -1);
            break;
        case 's':
            fight_or_move(0, 1);
            break;
        case 'a':
            fight_or_move(-1, 0);
            break;
        case 'd':
            fight_or_move(1, 0);
            break;
        case 'b':
            show_bag();
            break;
        case 'u':
            inventory_use();
            break;
        case 't':
            shop();
            break;
        case 'l': {
            char key[80];
            printf("搜索关键字: ");
            if (fgets(key, sizeof key, stdin)) {
                key[strcspn(key, "\r\n")] = 0;
                log_search(key);
            }
            break;
        }
        case 'h':
            auto_go();
            break;
        case '?':
            algorithm_menu();
            break;
        case 'n':
            leaderboard_record();
            depth++;
            hp = 30;
            make_dungeon();
            break;
        case 'q':
            running = false;
            break;
        default:
            help();
            break;
        }
        if (strncmp(line, "he", 2) == 0) {
            complete("he");
        }
        tick_effects();
        if (monster_n && rand() % 4 == 0 && hp > 0) {
            hp--;
            say("远处的怪物趁机袭击你。");
        }
    }

    puts(hp > 0 ? "你离开了地牢。" : "你倒在地牢中……");
    leaderboard();
    show_logs();
    free(bag.data);
    while (effects) {
        Effect *n = effects->next;
        free(effects);
        effects = n;
    }
    return 0;
}
