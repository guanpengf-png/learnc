# 电商类目树（纯递归）

用「左孩子 - 右兄弟」表示多级类目，**遍历 / 查找 / 统计 / 路径 / 销毁全是递归**。

## 运行

```bash
cd day8/category_tree
make
./category       # 自动演示
./category -i    # 交互：p / f / c / path / leaves / h / q
```

## 在练什么

| 递归函数 | 作用 |
|----------|------|
| `category_print` | 缩进打印整棵树 |
| `category_find` | 按名字查找 |
| `category_count_subtree` | 子树商品总数 |
| `category_height` | 树高 |
| `category_build_path` | 面包屑路径 |
| `category_print_leaves` | 列出叶子类目 |
| `category_free` | 后序释放整棵树 |

口诀：

- **往下** → 递归 `first_child`
- **同级** → 递归 `next_sibling`

## 建议练习

1. 看演示输出，对照源码里的 `build_demo_tree`
2. 用 lldb：`b category_print` → `run` → `p *c` → `n`
3. 自己改：给「苹果」加子类「配件」，看打印和路径怎么变
