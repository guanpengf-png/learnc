# learnc

从 C 语言入门到数据结构与算法的个人学习仓库。

包含 **52 个章节**、多份复习笔记，以及若干可以直接运行的完整项目：终端游戏、网页游戏、TCP 服务端/客户端、终端 Roguelike。

---

## 仓库结构

```
learnc/
├── chapters/                      # 01–52 章，每章一个或多个可独立编译的 .c
│   ├── 01-第一个程序/
│   ├── ...
│   ├── 31-预处理器/
│   ├── 32-数据结构概述/
│   ├── ...
│   ├── 52-并查集/
│   ├── 数据结构与算法索引.md      # 32–52 章的目录、复杂度速查、学习顺序
│   └── 32-52-数据结构与算法复习.md # 21 章复习笔记（原理 + 代码 + 易错点）
├── day5/                          # 每日练习 + 纯终端 Roguelike 项目
│   ├── roguelike/                 # 地牢探险（C11，单文件，Makefile）
│   └── README.md
├── day6/                          # 字符数组练习
├── missing-semester-2026/         # MIT Missing Semester 2026 中文学习笔记
├── pvz.c / pvz.html               # 植物大战僵尸：终端版 / 网页版
├── tank.c                         # 坦克大战（终端，A* / BFS / 堆 / 链表）
├── tcp_server.c / tcp_client.c    # TCP 回声服务器与客户端
├── answer.c                       # 课后习题：闰年、水仙花、月份天数等
└── 国际跳棋-题目与代码对照.md
```

---

## 快速开始

### 环境要求

- macOS 或 Linux
- `clang` 或 `gcc`（macOS 安装 Xcode Command Line Tools 即可）
- 网页版游戏只需要浏览器

Windows 建议使用 **WSL**，因为终端游戏依赖 `termios` 等 POSIX 接口。

### 编译任意章节

每个章节文件都是独立的，自带 `main`，可以直接编译运行：

```bash
clang -Wall -Wextra -O2 "chapters/37-哈希表/hash_table.c" -o /tmp/demo
/tmp/demo
```

建议加上内存检查，尤其是链表、树、哈希表这些用到 `malloc/free` 的章节：

```bash
clang -Wall -Wextra -g -fsanitize=address,undefined \
      "chapters/39-二叉搜索树/bst.c" -o /tmp/bst
/tmp/bst
```

VS Code 里打开某个 `.c` 文件后按 `Cmd+Shift+B`，会使用 `.vscode/tasks.json` 里的 clang 任务编译到同目录。

### 运行项目

| 项目 | 命令 |
|---|---|
| 植物大战僵尸（终端） | `clang -O2 -Wall -Wextra pvz.c -o pvz && ./pvz` |
| 植物大战僵尸（网页） | `open pvz.html` |
| 坦克大战 | `clang -O2 -Wall -Wextra tank.c -o tank && ./tank` |
| TCP 回声服务器 | `clang -O2 tcp_server.c -o tcp_server && ./tcp_server` |
| TCP 客户端 | `clang -O2 tcp_client.c -o tcp_client && ./tcp_client` |
| 课后习题 | `clang -O2 answer.c -o answer && ./answer` |
| Roguelike | `cd day5/roguelike && make && ./dungeon` |

> 终端游戏必须运行在**真正的终端**里（VS Code 集成终端可以），不要用 IDE 的 "Run" 按钮或输出面板。

---

## 章节导航

### 基础篇（01–31）

| 阶段 | 章节 |
|---|---|
| 入门 | [01 第一个程序](chapters/01-第一个程序/) · [02 编译流程](chapters/02-编译流程/) · [03 变量](chapters/03-变量/) · [04 常量](chapters/04-常量/) |
| 数据表示 | [05 进制](chapters/05-进制/) · [06 原码反码补码](chapters/06-原码反码补码/) · [07 数据类型](chapters/07-数据类型/) · [08 类型转换](chapters/08-类型转换/) |
| 控制流程 | [09 运算符](chapters/09-运算符/) · [10 分支](chapters/10-分支/) · [11 循环](chapters/11-循环/) · [12 跳转](chapters/12-跳转/) · [13 练习](chapters/13-练习/) · [14 断点调试](chapters/14-断点调试/) |
| 数组与函数 | [15 数组](chapters/15-数组/) · [16 字符数组](chapters/16-字符数组/) · [17 二维数组](chapters/17-二维数组/) · [18 函数](chapters/18-函数/) · [19 作用域](chapters/19-作用域/) · [20 static 与 extern](chapters/20-static与extern/) · [21 递归](chapters/21-递归/) · [22 系统函数](chapters/22-系统函数/) |
| 指针与复合类型 | [23 指针](chapters/23-指针/) · [24 指针与数组](chapters/24-指针与数组/) · [25 指针与函数](chapters/25-指针与函数/) · [26 枚举](chapters/26-枚举/) · [27 结构体](chapters/27-结构体/) · [28 共用体](chapters/28-共用体/) · [29 typedef](chapters/29-typedef/) |
| 内存与预处理 | [30 动态内存](chapters/30-动态内存/) · [31 预处理器](chapters/31-预处理器/) |

### 数据结构与算法（32–52）

| 章 | 内容 | 关键点 |
|---|---|---|
| [32 概述](chapters/32-数据结构概述/) | 逻辑结构 / 存储结构 | 顺序存储 vs 链式存储 |
| [33 动态数组](chapters/33-动态数组/) | 动态数组 | 扩容、`realloc`、均摊 O(1) |
| [34 链表](chapters/34-链表/) | 单链表 | 头插、二级指针删除 |
| [35 栈](chapters/35-栈/) | 顺序栈 | LIFO、括号匹配、撤销 |
| [36 队列](chapters/36-队列/) | 循环队列 | 假溢出、`(rear+1)%cap` |
| [37 哈希表](chapters/37-哈希表/) | 链地址法 | 冲突、装填因子、平均 O(1) |
| [38 二叉树](chapters/38-二叉树/) | 二叉树 | 前/中/后/层序遍历 |
| [39 二叉搜索树](chapters/39-二叉搜索树/) | BST | 有序、删除三种情况、退化 |
| [40 平衡二叉树](chapters/40-平衡二叉树/) | AVL | 平衡因子、四种旋转 |
| [41 堆与优先队列](chapters/41-堆与优先队列/) | 二叉堆 | 上浮/下沉、O(n) 建堆 |
| [42 排序](chapters/42-排序/) | 六种排序 | 复杂度、稳定性、原地 |
| [43 查找](chapters/43-查找/) | 线性 / 二分 | `lower_bound`、溢出坑 |
| [44 图](chapters/44-图/) | 图的存储与遍历 | 邻接矩阵/表、BFS/DFS |
| [45 最短路径](chapters/45-最短路径/) | Dijkstra | 松弛、非负权、路径还原 |
| [46 最小生成树](chapters/46-最小生成树/) | Prim / Kruskal | 切分性质、并查集判环 |
| [47 动态规划](chapters/47-动态规划/) | DP | 状态/转移/边界、背包、LIS |
| [48 字符串匹配](chapters/48-字符串匹配/) | KMP | `pi` 数组、重叠匹配 |
| [49 字典树](chapters/49-字典树/) | Trie | 前缀查询、自动补全 |
| [50 回溯](chapters/50-回溯/) | 回溯 | 撤销选择、剪枝、八皇后 |
| [51 贪心](chapters/51-贪心/) | 贪心 | 活动选择、分数背包、反例 |
| [52 并查集](chapters/52-并查集/) | Union-Find | 路径压缩、按秩合并 |

配套两份文档：

- [数据结构与算法索引](chapters/数据结构与算法索引.md)：目录、学习顺序、复杂度速查
- [32–52 复习笔记](chapters/32-52-数据结构与算法复习.md)：每章一句话 + 关键代码 + 易错点

---

## 项目介绍

### 植物大战僵尸

- **终端版** [`pvz.c`](pvz.c)：回合制，5×9 草坪，5 种植物、3 种僵尸、除草机、波次生成。
- **网页版** [`pvz.html`](pvz.html)：Canvas + 原生 JS，鼠标操作，带音效、粒子、动画，可直接用浏览器打开。

涉及：二维数组、动态数组、链表/数组实体管理、状态机、波次队列、碰撞检测。

### 坦克大战

[`tank.c`](tank.c)：纯终端 Battle City。

- 31×21 地图，玩家 + 3 种敌人 AI
- A* 加权寻路（小根堆优先队列）、BFS 寻路
- 单向链表子弹、指针网格 `occ[][]`、函数指针分派 AI
- `termios` 原始终端、非阻塞输入、ANSI 渲染

操作：`WASD`/方向键移动，空格开火，`Q` 退出。

### TCP 回声服务器

[`tcp_server.c`](tcp_server.c) / [`tcp_client.c`](tcp_client.c)：`socket → bind → listen → accept` 标准流程，`send_all` / `recv_all` 处理半包。

```bash
./tcp_server 9000        # 终端 1
./tcp_client 127.0.0.1 9000   # 终端 2
```

### 地牢探险（Roguelike）

[`day5/roguelike`](day5/roguelike/)：纯终端、C11、无第三方库。随机地牢、回合制战斗、背包、商店、日志搜索、AVL 排行榜。

```bash
cd day5/roguelike
make
./dungeon
```

代码按 32–52 章标注落点，读游戏的同时复习数据结构。详细说明见 [day5/README.md](day5/README.md)。

### 课后习题

[`answer.c`](answer.c)：闰年、水仙花数、月份天数、字母输出、交错级数求和。
[`chapters/13-练习/function_exercises.c`](chapters/13-练习/function_exercises.c)：质数、年内第几天、数组查找、实心三角形。

---

## 学习路线

1. **基础语法**：01 → 31，边看边改，每个文件都自己敲一遍。
2. **数据结构与算法**：32 → 52，先看每章顶部注释，再自己实现核心函数，最后对照代码。
3. **综合项目**：从 `pvz.c` 或 `tank.c` 挑一个读，尝试加功能（新植物、新敌人、存档）。
4. **工程工具**：[missing-semester-2026](missing-semester-2026/00-README.md) 里的 Shell、Git、调试、测试笔记。

推荐复习方式：

- 第一遍只看每章「一句话记忆」和复杂度表；
- 第二遍遮住代码，自己手写插入/删除/遍历/旋转；
- 第三遍只看易错点清单。

---

## 代码约定

- 每个章节 `.c` 都是独立程序，自带 `main` 和演示输出。
- 文件顶部用注释写清楚：知识点、原理、复杂度、易错点、应用场景。
- 使用 C99/C11，尽量只依赖标准库和 POSIX 接口。
- 内存分配必须检查返回值，`malloc/free` 成对出现。

---

## 注意事项

- 仓库里的 `tank`、`pvz`、`answer`、`*.dSYM/`、`build/` 是编译产物，不建议提交到 Git。可以加一个 `.gitignore`：

  ```gitignore
  *.dSYM/
  build/
  tank
  pvz
  answer
  ```

- `lean.c` 是早期手误（应为 `learn.c`），内容是最小 Hello World，保留作为起点记录。
- 终端游戏需要 TTY；在 IDE 的输出面板里运行会读不到键盘输入。
- Windows 原生环境没有 `termios`，终端类项目请使用 WSL。

---

## 参考资源

- [TheAlgorithms/C](https://github.com/TheAlgorithms/C)
- [attractivechaos/klib](https://github.com/attractivechaos/klib)
- [代码随想录](https://programmercarl.com/)
- [王道数据结构](https://www.bilibili.com/video/BV1b7411N798/)
- [MIT Missing Semester](https://missing.csail.mit.edu/)

---

## License

本仓库仅用于个人学习记录，未指定开源许可证。引用第三方代码时请遵守其原始许可。
