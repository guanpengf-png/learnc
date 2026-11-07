# gcc、CMake、GDB：编译、构建和调试

三个工具各管一件事：

| 工具 | 干什么 | 什么时候用 |
|---|---|---|
| **gcc / clang** | 把一个或几个 `.c` 编译成可执行文件 | 现在这种单文件练习 |
| **CMake** | 记下编译命令，一次构建整个项目 | 文件变多、要区分调试版和发布版 |
| **GDB / LLDB** | 让程序停在某一行，查看变量 | 结果不对，靠 `printf` 已经看不出来 |

关系是：先用 gcc 学会编译，文件多了交给 CMake 去调用 gcc，要查错再拿 GDB 或 LLDB 调试。CMake 不代替编译器，调试器也不能编译代码。

## 你这台 Mac 现在的情况

已经查过本机：

| 命令 | 状态 |
|---|---|
| `cc`、`gcc`、`clang` | 都已安装，而且是同一个程序：Apple clang 15 |
| `lldb` | 已安装，这是 macOS 自带的调试器 |
| `make` | 已安装 |
| `cmake` | 未安装 |
| `gdb` | 未安装 |

所以终端里输入 `gcc`，跑起来的仍是 clang。日常练习直接用 `cc` 或 `gcc` 都可以，参数一样。课程和 Linux 环境讲的调试器是 GDB；在这台电脑上，能马上用的是 LLDB。两边的思路相同，命令对照见最后一节。

还没装的两个工具：

```bash
brew install cmake gdb
```

macOS 上的 GDB 装完还要给它做代码签名，否则系统不允许它控制别的进程。签名步骤麻烦，而且 Apple 芯片上经常不稳定。学习阶段用 LLDB。GDB 命令仍然要认识，换到 Linux 或课程实验环境时就是那一套。

调试任何程序之前，编译时都要加 `-g`。没有它，调试器不知道某一行机器码对应源码的哪一行。

---

## 1. gcc：把 `.c` 变成程序

### 1.1 一条命令编译并运行

在源文件所在目录：

```bash
cd chapters/01-第一个程序
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 hello.c -o hello
./hello
```

`-o hello` 指定输出文件名。不加 `-o` 时，默认生成 `a.out`。

### 1.2 这几个参数分别做什么

| 参数 | 作用 |
|---|---|
| `-std=c11` | 按 C11 标准检查。课程代码用这个标准就够 |
| `-Wall -Wextra -Wpedantic` | 打开警告。警告往往就是以后的 bug |
| `-g` | 写入调试信息，给 LLDB / GDB 用 |
| `-O0` | 不做优化。优化后变量可能被删掉，单步会跳行 |
| `-o 名字` | 输出文件的名字 |

想看每一步产生什么，用前面学过的四步。在 `chapters/02-编译流程` 里：

```bash
sh steps.sh
```

对应的四条命令是：

```bash
gcc -std=c11 -E main.c -o main.i    # 预处理：展开头文件、删注释
gcc -std=c11 -S main.i -o main.s    # 编译：变成汇编
gcc -std=c11 -c main.s -o main.o    # 汇编：变成目标文件，还不能运行
gcc main.o -o main                  # 链接：接上 printf 所在的标准库
```

平时不用拆开，一条 `gcc hello.c -o hello` 会把四步都做完。

### 1.3 多个源文件

一个程序拆成两个文件时，把它们一起交给 gcc：

```bash
gcc -std=c11 -Wall -Wextra -g -O0 main.c util.c -o app
```

只重新编译改过的那一个文件时，先得到 `.o`，再链接：

```bash
gcc -std=c11 -Wall -Wextra -g -O0 -c main.c -o main.o
gcc -std=c11 -Wall -Wextra -g -O0 -c util.c -o util.o
gcc main.o util.o -o app
```

头文件不参与这条命令。`#include` 会在预处理时把内容贴进来。

### 1.4 和编辑器里那个运行按钮的关系

当前工作区的生成任务用的就是 clang，并且带了调试信息。它大致等价于：

```bash
clang -g 当前文件.c -o 当前文件名
```

按钮适合单文件。要同时编译多个文件、统一警告等级、分开调试版和发布版时，改用 CMake。

---

## 2. CMake：把编译命令写成一份清单

文件一多，没人想每次手打那一长串 `gcc`。CMake 读 `CMakeLists.txt`，生成真正的构建文件，再调用 gcc 或 clang。

### 2.1 安装

```bash
brew install cmake
cmake --version
```

### 2.2 最小例子

假设目录是这样：

```text
demo/
  CMakeLists.txt
  main.c
```

`main.c` 就是普通的 `hello` 程序。`CMakeLists.txt` 写成：

```cmake
cmake_minimum_required(VERSION 3.16)
project(demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(demo main.c)

target_compile_options(demo PRIVATE -Wall -Wextra -Wpedantic)
```

`add_executable(demo main.c)` 的意思是：用 `main.c` 生成一个名叫 `demo` 的程序。以后增加 `util.c`，把它写进这个列表即可，不用改命令。

### 2.3 配置、编译、运行

在 `demo` 目录执行：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/demo
```

这三步的分工：

1. `cmake -S . -B build`：阅读当前目录的 `CMakeLists.txt`，把生成的构建文件放到 `build/`。源码目录保持干净。
2. `-DCMAKE_BUILD_TYPE=Debug`：调试版，自动带 `-g`，并关闭优化。发布版把 `Debug` 换成 `Release`。
3. `cmake --build build`：真正调用编译器。改完代码后，重复这一条就行，不必重新配置。

常用后续命令：

```bash
cmake --build build --clean-first   # 全部重编
rm -rf build                         # 配置改乱了，删掉重建
```

`build/` 是生成物，不要提交到 git。

### 2.4 多个源文件和头文件目录

```cmake
add_executable(demo
    main.c
    util.c
)

target_include_directories(demo PRIVATE include)
```

`include/` 里放自己的 `.h`。这样源文件写 `#include "util.h"`，不用关心头文件的相对路径。

### 2.5 什么时候不必用 CMake

只有一个 `.c`、命令不超过一行时，直接 `gcc` 更快。CMake 解决的是「命令会变长、文件会变多、别人也要能编译」这件事。

---

## 3. 调试：让程序停下来

调试器做的事就四件：

1. 在某一行停下（断点）
2. 一次执行一行（单步）
3. 看变量现在的值
4. 看函数是从哪里调用过来的（调用栈）

下面用 `chapters/14-断点调试/debug_sum.c`。它的循环是：

```c
int sum = 0;
for (int i = 0; i < 10; i++) {
    sum += i;
    printf("i = %d, sum = %d\n", i, sum);
}
```

### 3.1 先编出带调试信息的程序

```bash
cd chapters/14-断点调试
gcc -std=c11 -Wall -Wextra -g -O0 debug_sum.c -o debug_sum
```

漏了 `-g`，调试器只能看见内存地址，看不见变量名和行号。漏了 `-O0`、用了默认优化时，单步会跳过某些行，`sum` 也可能显示不出来。

### 3.2 在这台 Mac 上用 LLDB

```bash
lldb ./debug_sum
```

进入 `(lldb)` 提示符后：

```text
(lldb) breakpoint set --file debug_sum.c --line 18
(lldb) run
(lldb) frame variable
(lldb) print sum
(lldb) next
(lldb) print sum
(lldb) continue
(lldb) quit
```

断点要打在真正会执行的语句上，例如 `sum += i;`。打在空行、注释或右花括号上时，LLDB 会把它挪到下一条能执行的语句，这是正常的。

在 Cursor 里可以不记命令：打开 `debug_sum.c`，点击行号左侧出现红点，按 F5 开始，按 F10 执行下一行。当前的调试配置类型就是 `lldb`，程序路径是 `build/Debug/outDebug`。如果 F5 找不到程序，先用编辑器的生成按钮编译一次，或把配置里的 `program` 改成你刚刚 `gcc -o` 出来的那个文件。

### 3.3 GDB 命令

GDB 是 Linux 和多数 C 课程使用的调试器。装好之后：

```bash
gdb ./debug_sum
```

提示符是 `(gdb)`。同一段循环可以这样看：

```text
(gdb) break debug_sum.c:18
(gdb) run
(gdb) print sum
(gdb) print i
(gdb) next
(gdb) print sum
(gdb) continue
(gdb) quit
```

`run` 的参数会原样传给程序。例如程序需要命令行参数时写 `run 10 20`。需要键盘输入时，程序停住后直接打字，和平时运行一样。

### 3.4 两边命令对照

| 想做的事 | GDB | LLDB |
|---|---|---|
| 启动并停在 `main` | `start` | `process launch --stop-at-entry`，或先 `b main` 再 `run` |
| 运行 | `run` | `run` |
| 在某行停下 | `break 文件.c:行号` | `breakpoint set --file 文件.c --line 行号` |
| 在函数入口停下 | `break main` | `breakpoint set --name main` |
| 查看断点 | `info breakpoints` | `breakpoint list` |
| 删除 1 号断点 | `delete 1` | `breakpoint delete 1` |
| 执行下一行，不进入函数 | `next` | `next` |
| 执行下一行，进入函数 | `step` | `step` |
| 继续跑到下一个断点 | `continue` | `continue` |
| 从当前函数返回 | `finish` | `finish` |
| 看变量 | `print sum` | `print sum` 或 `frame variable` |
| 每停一次都显示 | `display sum` | `target stop-hook add -o "frame variable sum"` |
| 调用栈 | `backtrace` | `bt` |
| 看附近源码 | `list` | `source list` |
| 退出 | `quit` | `quit` |

简写在 GDB 里很常见：`b` 是 `break`，`r` 是 `run`，`n` 是 `next`，`s` 是 `step`，`c` 是 `continue`，`p` 是 `print`，`bt` 是 `backtrace`，`q` 是 `quit`。

### 3.5 一次具体的追查

`sum` 的前几步应该是 0、1、3、6、10。如果某次变成了别的数：

1. 断点打在 `sum += i;`
2. `run`，第一次停下时 `print i` 和 `print sum`
3. 反复 `next`，直到某一步和手算不一致
4. 停住的这一行就是计算出错的地方

调用栈用在程序已经崩了的时候。GDB 里运行到崩溃后输入 `backtrace`，最上面一帧就是崩溃所在的函数，往下是谁调用了它。LLDB 里同样输入 `bt`。

---

## 4. 推荐的日常顺序

单文件练习：

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -O0 文件.c -o 程序
./程序
```

结果不对，再：

```bash
lldb ./程序
```

一个项目里有多个 `.c` 之后，写 `CMakeLists.txt`，然后固定用：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
lldb ./build/程序名
```

警告不要放过。`-Wall -Wextra` 报出来的内容，比程序跑起来之后再查更容易修。
