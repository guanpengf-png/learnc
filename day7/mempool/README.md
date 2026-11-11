# 简易内存池（空闲块链表）

## 运行

```bash
cd day7/mempool
make
./mempool          # 自动演示：分裂 / 碎片 / 合并
./mempool -i       # 交互：a <size> / f <句柄> / d / q
```

## 在练什么

| 概念 | 代码 |
|------|------|
| 块头 + 指针算术 | `Block`、`user_from_block` / `block_from_ptr` |
| 空闲链表 | `free_head`、`freelist_push` / `freelist_remove` |
| 物理相邻链 | `phys_prev` / `phys_next` |
| 分裂 | `block_split` |
| 合并 | `block_coalesce`（先右后左） |
| 分配 / 释放 | `pool_alloc`（首次适应）、`pool_free` |

## 建议看输出时想的问题

1. 连续 `alloc` 后，为什么物理块变多？  
2. 只 `free` 中间那块，为什么出现 FREE 夹在 BUSY 之间？  
3. 再 `free` 两边后，块数为什么变少、size 为什么变大？
