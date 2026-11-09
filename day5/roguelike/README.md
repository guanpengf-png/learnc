# 地牢探险（纯终端 Roguelike）

对照 `chapters/32-52-数据结构与算法复习.md` 的教学游戏：随机地牢、回合制战斗、背包、商店、命令补全、排行榜。

## 编译与运行

```bash
cd day5/roguelike
make
./dungeon
```

## 操作

| 键 | 作用 |
|---|---|
| `wasd` | 移动 / 近战 |
| `b` / `1`/`2`/`3` | 背包；按价格(归并) / 稀有度 / 重量排序 |
| `u` | 喝治疗药水 |
| `t` | 商店（BST 陈列 + 贪心找零演示） |
| `h` | Dijkstra 自动前往机关门房间 |
| `p` | 机关门错位排列解谜 |
| `z` | 悔棋（栈） |
| `l` | KMP 搜日志 |
| `c` / 前缀+`?` | Trie 补全 |
| `n` | 下一层 |
| `r` | AVL 排行榜 |
| `?` | 帮助 + 0-1 背包 / LIS / 编辑距离 |
| `q` | 退出 |

图例：`@` 你 · `g` 怪 · `*` 物品 · `#` 墙 · `.` 地 · `~` 泥沼 · `^` 陷阱 · `+` 门

## 章节 ↔ 模块

| 章 | 文件 | 游戏用途 |
|---|---|---|
| 33 | `ds/vector.c` | 怪物池空槽复用 |
| 34 | `ds/list.c` | buff/debuff 链 |
| 35 | `ds/stack.c` | DFS 连通检查、悔棋 |
| 36 | `ds/queue.c` | 事件队列、BFS 视野 |
| 37 | `ds/hash.c` | 物品库、坐标空间哈希 |
| 38 | `ds/bsp.c` | BSP 房间生成 |
| 39 | `ds/bst.c` | 商店按价格 |
| 40 | `ds/avl.c` | 多维排行榜 |
| 41 | `ds/heap.c` | 行动调度、Dijkstra open |
| 42 | `ds/sort.c` | 背包 qsort / 归并 |
| 43 | `ds/search.c` | 二分 / 线性查找 |
| 44 | `ds/graph.c` | 房间邻接图 |
| 45 | `ds/path.c` | 加权地形寻路 |
| 46 | `ds/mst.c` | Kruskal 走廊 |
| 47 | `ds/dp.c` | 0-1 背包 / LIS / 编辑距离 |
| 48 | `ds/kmp.c` | 日志搜索、敏感词 |
| 49 | `ds/trie.c` | 命令补全 |
| 50 | `ds/backtrack.c` | 机关门排列 |
| 51 | `ds/greedy.c` | 找零反例 |
| 52 | `ds/uf.c` | Kruskal / 阵营合并 |

串联逻辑在 `game/game.c`。
