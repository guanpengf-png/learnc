# 教学版贪吃蛇（结构体 + 指针）

回合制终端蛇：每输入一个字符走一格，方便对照代码看指针。

## 运行

```bash
cd day6/snake
make
./snake
```

`wasd` / `hjkl` 转向，单独回车=按原方向走，`q` 退出。

## 知识点对照

| C 基础 | 代码里在哪 |
|--------|------------|
| `struct` / `typedef` | `Point`、`Snake`、`Game`、`KeyBind` |
| `p->member` | 几乎所有 `Snake *` / `Game *` 函数 |
| 指针传参改数据 | `game_tick(Game *)`、`snake_step(Snake *)` |
| `malloc` / `free` | `game_init` 的地图、`snake_init` 的身体 |
| `realloc` + **二级指针** | `snake_reserve(Point **body, ...)` |
| 指针算术遍历 | `for (p = body; p < body + len; p++)` |
| 一维模拟二维 | `cells + y * MAP_W + x`，`cell_at` 返回 `char *` |
| `const` 只读指针 | `snake_hits_self(const Snake *, const Point *)` |
| 函数指针 | `DirHandler` + `BINDS[]` 按键表 |
| `memset` / 数组首地址 | `clear_cells` |

## 建议自学顺序

1. 先玩几局，搞清 `@` 头、`o` 身、`*` 食物  
2. 读 `Point` / `Snake` / `Game` 三个结构体  
3. 跟一次 `snake_step`：头怎么插、尾巴怎么挪  
4. 盯 `snake_reserve`：为什么必须是 `Point **`  
5. 改一改：例如把 `grow` 得分从 10 改成 5，或地图加大  

吃完这个，再学链表版蛇身会非常自然（`body` 从数组指针换成节点指针）。
