# Debugging and Profiling（调试与性能分析）

原讲义：https://missing.csail.mit.edu/2026/debugging-profiling/

## 本讲目标

编程的一条黄金法则是：代码不会按你「期望」的去做，而会按你「告诉它」的去做。本讲覆盖两类核心技能——**调试（debugging）** 与 **性能分析（profiling）**：如何定位错误行为，以及如何找出占用 CPU/内存等资源的热点并有依据地优化。

---

## 知识点

### 一、Printf 调试与日志（Logging）

Brian Kernighan 有句名言：最有效的调试工具仍是仔细思考，加上审慎放置的打印语句。

两种常用路径：

1. **Printf 调试**：在可疑位置加 `print`，迭代收集信息，直到弄清根因。
2. **日志框架**：本质上是「更谨慎的打印」，通常提供：
   - 把日志（或子集）导向不同输出位置；
   - 严重级别（INFO、DEBUG、WARN、ERROR 等）及按级别过滤；
   - 结构化日志，便于事后提取字段。

实践建议：写代码时主动埋日志；用 print 修好 bug 后，常把临时打印**改成正式日志**再删，以便同类问题再次出现时已有诊断信息。

**第三方日志**：许多程序支持 `-v` / `--verbose`（可重复以增加详细度）。服务（数据库、Web 服务器等）日志常在 Linux 的 `/var/log/`；systemd 服务可用 `journalctl -u <service>`。第三方库也可能通过环境变量或配置开启 debug 日志。

### 二、调试器（Debuggers）

适用场景：不确定该打印什么；bug 难复现；修改/重启成本高（长启动、复杂状态）。

调试器可：在指定行停住、单步执行、崩溃后查看变量、按条件断点等。通用工具如 `gdb`（GNU）、`lldb`（LLVM）可调试任意原生二进制；语言专用如 Python `pdb`、Java `jdb`。

`gdb` 是 C/C++/Rust 等编译型语言的事实标准，可查看寄存器、栈、程序计数器等机器状态。

常用 GDB 命令：

| 命令 | 含义 |
|------|------|
| `run` | 启动程序 |
| `b {function}` / `b {file}:{line}` | 设断点 |
| `c` | 继续 |
| `step` / `next` / `finish` | 步入 / 步过 / 步出 |
| `p {variable}` | 打印变量 |
| `bt` | 回溯（调用栈） |
| `watch {expression}` | 值变化时中断 |

可用 TUI 模式：`gdb -tui` 或在 GDB 内 `Ctrl-x a`，分屏显示源码与命令行。

### 三、录制-重放与反向调试（rr）

**Heisenbug**（海森堡 bug）：一观察就消失或行为改变——竞态、时序依赖、特定系统条件下才出现。传统调试常无用，因为再跑一次行为就变了（例如 print 拖慢后竞态不再发生）。

**Record-replay**：录制一次执行，之后可确定性重放；还可**反向**执行，精确定位出错点。

Linux 上的 **rr** 可录制并配合 GDB 重放。反向命令包括：

- `reverse-continue` (`rc`)：反向跑到断点
- `reverse-step` (`rs`)：反向一步
- `reverse-next` (`rn`)：反向步过函数调用
- `reverse-finish`：反向跑到进入当前函数之前

典型流程（崩溃）：跑到崩溃 → 检查损坏状态 → 对损坏变量设 watchpoint → `reverse-continue` 找到是谁改坏的。

适用：间歇失败的 flaky 测试、竞态与线程 bug、难复现崩溃、「想回到过去」的任意 bug。

限制：仅 Linux，需硬件性能计数器；多数不暴露计数器的 VM（如多数 AWS EC2）不可用；不支持 GPU。macOS 可关注 Warpspeed。rr 会序列化线程调度，依赖精确时序的竞态可能录不到；一旦录到失败运行即可可靠重放。非并发 bug 上 rr 最强。

### 四、系统调用跟踪

程序通过系统调用请求内核服务（开文件、分配内存、创建进程等）。跟踪可揭示挂起原因、访问了哪些文件、在何处等待。

#### strace（Linux）与 dtruss（macOS）

`strace` 观察程序的每次系统调用；macOS/BSD 用 `dtruss`（包装 `dtrace`）。深入材料可参考 Julia Evans 的 strace zine。

#### bpftrace 与 eBPF

**eBPF** 允许在内核中运行沙箱程序；**bpftrace** 提供高层语法。常见用途：调查系统调用、聚合（计数、延迟统计）、检查/过滤参数。也可用 C + `bcc` 工具链（如 `biosnoop`、`opensnoop`）。

对比：`strace` 上手快；需要**更低开销**、跟踪内核函数、做聚合时用 `bpftrace`。bpftrace 需 `root`，默认监控整个内核；可用 `comm`/`pid` 过滤，或 `-c` 从启动起跟踪指定命令（`cpid` = 子进程 PID）。

#### 网络调试

`tcpdump` / Wireshark 抓包分析。HTTPS 加密后 tcpdump 价值下降；**mitmproxy** 可作拦截代理查看加密流量。浏览器开发者工具 Network 标签对 Web 应用 HTTPS 往往最省事（解密后的请求/响应、头、时序）。

### 五、内存调试

缓冲区溢出、use-after-free、泄漏等往往不立即崩溃，而是稍后才出问题。

#### Sanitizers（需重编译）

编译器插桩，运行时检测。**AddressSanitizer (ASan)** 可检测：栈/堆/全局溢出、use-after-free、use-after-return、泄漏。

其他：

- **TSan**（`-fsanitize=thread`）：多线程数据竞争
- **MSan**（`-fsanitize=memory`）：读未初始化内存
- **UBSan**（`-fsanitize=undefined`）：未定义行为如整数溢出

足够快，可用于 CI 与日常开发。

#### Valgrind（无需重编译）

在类虚拟机中运行程序检测内存错误，更慢。适用：无源码、无法重编译（第三方库）、需要 sanitizer 没有的特定工具。Valgrind 也是受控执行环境，profiling 部分还会再用到。

### 六、用 AI 辅助调试

LLM 擅长：解释晦涩报错（C++ 模板、Rust borrow checker）；跨语言/抽象边界（FFI、构建、依赖）；把模糊症状关联到可能根因；分析 crash dump / stack trace。

**调试符号**：有意义的栈回溯需 `-g`（DWARF）；`-fno-omit-frame-pointer` 让栈更可靠（尤其对 profiler）。无这些时可能只见地址或不完整。对原生程序（C++/Rust）比 Python/Java 更关键。

局限：可能幻觉；可能建议「掩盖」而非修好；须用真实调试工具验证；是补充而非替代对代码的理解。这与「开发环境」讲中的通用 AI 编程能力不同，这里专指调试助手。

### 七、Profiling：计时与资源监控

功能正确但吃光 CPU/内存仍不够。应避免过早优化，用 profiler/监控找到热点再优化。

#### `time`：Real / User / Sys

- **Real**：墙钟时间（含等待）
- **User**：用户态 CPU 时间
- **Sys**：内核态 CPU 时间

若 Real 远大于 User+Sys，往往在等 I/O 或网络。

#### 资源监控工具

| 工具 | 用途 |
|------|------|
| `htop`（亦有 `btop`） | 进程统计；`<F6>` 排序、`t` 树、`h` 切换线程 |
| `iotop` | 实时 I/O |
| `free` | 空闲/已用内存 |
| `lsof` | 进程打开的文件 |
| `ss` | 网络连接；如 `ss -tlnp \| grep :8080` 查端口占用 |
| `nethogs` / `iftop` | 按进程网络用量 |

### 八、可视化性能数据

人眼从图中找模式远快于数字表。调试打印时尽量打成可绘图格式（如 CSV：`timestamp,value`），或结构化 JSON。

- **gnuplot**：命令行快速出图
- **matplotlib / ggplot2**：迭代探索；ggplot2 的 facet 可按类别分子图

用例：延迟随时间揭示周期性变慢（GC、cron、流量）；插入时间图暴露复杂度（向量扩容尖峰）；按维度 facet 发现「全系统」问题其实只在一类请求。

### 九、CPU Profiler：采样 vs 跟踪

- **Tracing profiler**：记录每次函数调用
- **Sampling profiler**：周期性采样栈（常约每毫秒）；开销低，生产更常用

#### perf（采样）

无需重编译。`perf stat` 给概览；`perf record -g` 后可用 **flame graph**（Y 轴调用层次，X 轴与时间成比例，可点击缩放）。也可用 Speedscope、Perfetto。

#### Valgrind Callgrind（跟踪）

精确调用次数与 caller/callee 关系；可用 `callgrind_annotate` 或 `kcachegrind`。比采样慢；可选 `--cache-sim=yes` 模拟缓存。

语言专用：Python `cProfile`/`py-spy`，Go `go tool pprof`，Rust `cargo-flamegraph`（也适用于一般编译程序）。

### 十、内存 Profiler 与基准测试

- **Massif**：堆内存随时间；`ms_print` 查看。Python 有 `memory-profiler`（按行）。
- **hyperfine**：对比命令行程序性能（warmup、均值、标准差）。
- Web：浏览器内置 profiler（Firefox Profiler、Chrome DevTools）。

---

## 代码/命令示例

### rr 录制与重放

```bash
# Record a program execution
rr record ./my_program

# Replay the recording (opens GDB)
rr replay
```

**作用：** 确定性录制一次执行，再用 GDB 接口重放，并支持 `reverse-*` 反向调试以定位损坏点。

### strace 系统调用跟踪

```bash
# Trace all system calls
strace ./my_program

# Trace only file-related calls
strace -e trace=file ./my_program

# Follow child processes (important for programs that start other programs)
strace -f ./my_program

# Trace a running process
strace -p <PID>

# Show timing information
strace -T ./my_program
```

**作用：** 观察程序对内核的系统调用；`-e` 过滤类型，`-f` 跟随子进程，`-p` 附着已运行进程，`-T` 显示每次调用耗时。

### bpftrace 聚合与过滤

```bash
# Trace file opens system-wide (prints immediately)
sudo bpftrace -e 'tracepoint:syscalls:sys_enter_openat { printf("%s %s\n", comm, str(args->filename)); }'

# Count system calls by name (prints summary on Ctrl-C)
sudo bpftrace -e 'tracepoint:syscalls:sys_enter_* { @[probe] = count(); }'

# Filter by command name (prints summary on Ctrl-C)
sudo bpftrace -e 'tracepoint:syscalls:sys_enter_* /comm == "bash"/ { @[probe] = count(); }'

# Trace a specific command from startup using -c (cpid = child PID)
sudo bpftrace -e 'tracepoint:syscalls:sys_enter_* /pid == cpid/ { @[probe] = count(); }' -c 'ls -la'
```

**作用：** 用 eBPF 在内核侧低开销跟踪/聚合系统调用；可按进程名或 `-c` 启动的子进程 PID 过滤。

### tcpdump 抓包

```bash
# Capture packets on port 80
sudo tcpdump -i any port 80

# Capture and save to file for Wireshark analysis
sudo tcpdump -i any -w capture.pcap
```

**作用：** 捕获网络包以便分析；`-w` 保存为 pcap 供 Wireshark 离线查看。

### AddressSanitizer 编译运行

```bash
# Compile with AddressSanitizer
gcc -fsanitize=address -g program.c -o program
./program
```

**作用：** 用 ASan 在运行时检测缓冲区溢出、use-after-free、泄漏等内存错误（配合 `-g` 便于定位）。

### Valgrind 泄漏检查

```bash
valgrind --leak-check=full ./my_program
```

**作用：** 无需重编译即可检测内存错误与泄漏；适合无源码或无法用 sanitizer 的场景。

### time 区分 Real/User/Sys

```bash
$ time curl https://missing.csail.mit.edu &> /dev/null
real	0m0.272s
user	0m0.079s
sys	    0m0.028s
```

**作用：** 区分墙钟时间与 CPU（用户态+内核态）时间；上例 Real≈300ms 而 CPU 仅约 107ms，其余在等网络。

### gnuplot 快速绘图

```bash
# Plot a simple CSV with timestamp,value
gnuplot -e "set datafile separator ','; plot 'latency.csv' using 1:2 with lines"
```

**作用：** 从 CSV 快速画出延迟等时间序列，便于发现尖峰与趋势。

### perf stat / flame graph

```bash
$ perf stat ./slow_program

# Record profile
perf record -g ./my_program

# Generate flame graph (requires flamegraph scripts)
perf script | stackcollapse-perf.pl | flamegraph.pl > flamegraph.svg
```

**作用：** `perf stat` 给出硬件计数器概览；`perf record -g` 采样调用栈，再经 flamegraph 脚本生成火焰图，直观看到热点函数层次。

### Callgrind 与 Massif

```bash
# Run with callgrind
valgrind --tool=callgrind ./my_program

# Analyze with callgrind_annotate (text) or kcachegrind (GUI)
callgrind_annotate callgrind.out.<pid>
kcachegrind callgrind.out.<pid>

valgrind --tool=massif ./my_program
ms_print massif.out.<pid>
```

**作用：** Callgrind 精确跟踪调用与指令计数；Massif 分析堆内存随时间变化，辅助找泄漏与过度分配。

### hyperfine 基准对比

```bash
$ hyperfine --warmup 3 'fd -e jpg' 'find . -iname "*.jpg"'
```

**作用：** 对两条命令做带 warmup 的统计对比，输出均值、方差与相对加速比。

### 练习中的 corruption / UAF / slow 示例

```c
// corruption.c 片段：curve_scores 循环到 i < 4，越界写入 scores[3]，
// 破坏相邻 students[1].id
void curve_scores(int student_idx, int curve) {
    for (int i = 0; i < 4; i++) {
        students[student_idx].scores[i] += curve;
    }
}
```

**作用：** 演示越界写导致「只改 student 0 却损坏 student 1」；配合 `rr` + watchpoint + `reverse-continue` 精确定位。

```c
// uaf.c：free 后仍写 greeting[0]
free(greeting);
greeting[0] = 'J';
```

**作用：** 展示 use-after-free；无 sanitizer 可能「看起来正常」，ASan 会明确报错。

```bash
gcc -g -O2 slow.c -o slow -lm
perf record -g ./slow
perf report
```

**作用：** 对含 `sin`/`cos` 热循环的程序采样，用 `perf report` / 火焰图查看时间花在何处。

---

## 练习要点

### Debugging

1. **有 bug 的归并排序**：`merge` 中 `else` 分支误用 `right[i]`（应为 `right[j]`）。用断点单步 `merge`，测试向量 `[3,1,4,1,5,9,2,6]` → `[1,1,2,3,4,5,6,9]`。
2. **rr + corruption.c**：`gcc -g` 编译；`rr record` / `rr replay`；对 `students[1].id` 设 watchpoint，损坏后 `reverse-continue` 找越界写。
3. **ASan + uaf.c**：先无 sanitizer 跑（可能「正常」），再 `-fsanitize=address -g` 看报错并修复。
4. **strace/dtruss**：跟踪 `ls -l` 等，观察打开了哪些文件。
5. **LLM**：粘贴晦涩编译错误或 strace/ASan 输出，请其解释，并用工具验证。

### Profiling

1. `perf stat`：理解各计数器含义。
2. `slow.c` + `perf record -g` + `perf report` / 火焰图。
3. `hyperfine` 对比两实现（如 `find` vs `fd`）。
4. `htop` 监控；`taskset --cpu-list 0,2 stress -c 3`——为何用不了三颗 CPU？（亲和性只允许 0 和 2）。
5. `python -m http.server 4444` + `ss -tlnp | grep 4444` 找占用进程并 `kill`。

---

## 延伸阅读

- 讲义本页：https://missing.csail.mit.edu/2026/debugging-profiling/
- Julia Evans：strace zine（讲义提及，深入 strace）
- Speedscope：交互式火焰图查看
- Perfetto：系统级分析
- Firefox Profiler / Chrome DevTools：Web 性能分析文档
- Warpspeed：macOS 上与 rr 相关的替代方向（讲义提及）
