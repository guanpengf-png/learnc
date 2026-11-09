# Course Overview + Introduction to the Shell

原讲义：https://missing.csail.mit.edu/2026/course-shell/

## 本讲目标

- 理解课程动机：补上 CS 课程里常缺的「工具使用」这一学期内容。
- 认识 Shell 相对 GUI / 语音 / LLM 接口的优势：可组合、可编程、可自动化。
- 掌握在 Shell 中导航文件系统：提示符、`cd`/`pwd`、绝对/相对路径、`.`/`..`。
- 理解 `$PATH` / `which`，并能用 `ls`、`cat`、`grep`、`sed`、`find`、`awk` 等基础工具处理文本与文件。
- 学会管道 `|`、重定向、`tee`，以及 `if`/`while`/`for`、命令替换 `$()`、shebang 与 `set -euo pipefail`。
- 了解现代替代工具：`tldr`、`zoxide`、`eza`、`bat`、`ripgrep`、`fd`。

## 知识点

### 课程概览

- 授课者：Anish、Jon、Jose（前 MIT 学生）；联系：`missing-semester@mit.edu`。
- 课程免费开放讲义与录像；适合当作多数 CS 课里缺失的「工具学期」。
- 结构：9 次约 1 小时讲座；讲义在线，课堂 demo 可能不在讲义中；每次有练习。
- 提问渠道：OSSU Discord `#missing-semester-forum`，或发邮件。

### Shell 是什么 / 为何重要

- 计算机有多种接口（GUI、语音、AR/VR、LLM），覆盖约 80% 场景，但往往只能做「已设计好的操作」。
- Shell 是文本接口：运行程序、给输入、以半结构化方式查看输出；几乎所有平台都有。
- 终端（terminal）是 Shell 的可视化界面；Shell 提示符（prompt）是你输入命令的地方。
- Linux/macOS 常见 Bash；Windows 的 cmd/PowerShell 不是本课重点——应使用 WSL 或 Linux 虚拟机。
- 其他 Shell（fish、zsh）更友好，但概念与 Bash 相通；本讲以 Bash 为主。
- Shell 更快、可组合程序自动化任务；对开源软件安装、CI、排错都很关键。

### 导航与路径

- 提示符示例：`missing:~$` —— 主机名 `missing`，当前目录 `~`（家目录），`$` 表示非 root。
- `man`：查手册；多数命令也支持 `--help`。
- `cd`：切换目录（内建命令，`which cd` 通常找不到独立程序）；`pwd` / `echo $PWD`：打印当前目录。
- 绝对路径：以 `/` 开头，从文件系统根开始。
- 相对路径：不以 `/` 开头，相对于当前工作目录解析。
- `.`：当前目录；`..`：父目录。
- Tab 可自动补全路径。

### 程序如何被找到

- `$PATH`：冒号分隔的目录列表；Shell 按顺序查找要执行的程序名。
- `which cmd`：显示实际执行的文件路径。
- 也可直接给出可执行文件的完整路径，绕过 `$PATH`。

### 常用命令行工具

- `date`：日期时间；`echo`：打印参数（按空白拆分参数；含空格用引号或 `\` 转义）。
- `ls`：列目录；`cat`：打印文件；`sort`：排序行；`uniq`：去掉**连续**重复行。
- `head` / `tail`：文件头/尾若干行。
- `grep pattern file`：按正则匹配行；`-r` 可递归目录。
- `sed`：可编程文件编辑；常见：原地全局替换。
- `find`：按条件递归找文件（类型、名字、修改时间、大小、`-exec` 等）。
- `awk`：解析结构化文本；常见：打印某列；`-F,` 指定分隔符。

### 管道、重定向与 Shell 语言

- `|`：把前一个程序的 stdout 接到后一个的 stdin（多数程序无文件参数时读 stdin）。
- `>file`：覆盖写入 stdout；`>>file`：追加；`<file`：从文件读 stdin。
- `tee`：一边继续往 stdout 传，一边写入文件。
- `if command1; then ...; fi`：看 `command1` 是否成功（非错误）；常用 `test`/`[`/`[[`]]`。
- `while`：条件成功则循环；`for varname in ...`：遍历列表。
- `$()`：命令替换（优先于旧式反引号，可嵌套）。
- shebang（`#!/bin/bash` 等）：按指定解释器执行脚本。
- `set -euo pipefail`：失败即退出、未定义变量报错、管道中失败也算失败。
- 后台任务 `&`、重定向 `2>&1`、算术扩展等在「不稳定测试」示例脚本中综合出现。
- 写 Shell 时强烈建议用 `shellcheck`；复杂脚本可考虑改用 Python 等。

### 现代替代工具（讲义建议）

| 传统 | 现代替代 | 说明 |
|------|----------|------|
| `man` | `tldr` | 展示常见用法示例 |
| `cd` | `zoxide`（`z`） | 记住常去路径，少打字 |
| `ls` | `eza` | 更友好的列目录 |
| `cat` | `bat` | 语法高亮与滚动 |
| `grep` | `ripgrep`（`rg`） | 更快；默认递归当前目录 |
| `find` | `fd` | 更人性化（可移植性较弱） |

## 代码/命令示例

### 打开终端后的提示符

```
missing:~$
```

**作用：** 说明提示符里主机名、当前目录（`~`）与非 root 标记（`$`）的含义。

### 运行程序与带参数

```
missing:~$ date
Fri 10 Jan 2020 11:49:31 AM EST
missing:~$ echo hello
hello
```

**作用：** 展示最基本的「执行程序」以及「第一个词是程序名、后续是参数」的解析方式。

### 用 `cd` 导航与 `~` / `/`

```
missing:~$ cd /bin
missing:/bin$ cd /
missing:/$ cd ~
missing:~$
```

**作用：** 练习绝对路径切换，并观察提示符中工作目录的变化。

### 相对路径

```
missing:~$ cd /
missing:/$ cd bin
missing:/bin$
```

**作用：** 对比绝对路径与相对路径：`bin` 相对于 `/` 解析为 `/bin`。

### `.` 与 `..`

```
missing:~$ cd /
missing:/$ cd bin/../bin/../bin/././../bin/..
missing:/$
```

**作用：** 演示每个目录都有的特殊分量：`.` 当前目录、`..` 父目录。

### `$PATH` 与 `which`

```
missing:~$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
missing:~$ which echo
/bin/echo
missing:~$ /bin/echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
```

**作用：** 说明 Shell 如何按 `$PATH` 查找程序；可用完整路径直接执行。

### 列出 `$PATH` 中的程序

```
missing:~$ ls /bin
```

**作用：** 通过列出 `$PATH` 中的目录内容，了解系统上可执行的程序集合。

### `sed` 原地全局替换

```
missing:~$ sed -i 's/pattern/replacement/g' file
```

**作用：** 最常见的 `sed` 用法：`-i` 原地改文件；`s///g` 对每行全局替换；`pattern` 为正则。

### `find`：按类型、名字、修改时间找文件

```
missing:~$ find ~/Downloads -type f -name "*.zip" -mtime +30
```

**作用：** 找 Downloads 里超过 30 天的 ZIP 文件；引入 glob（`*.zip`）与 `find` 条件组合。

### `find`：按大小并用 `-exec`

```
missing:~$ find ~ -type f -size +100M -exec ls -lh {} \;
```

**作用：** 找大于 100M 的文件并 `ls -lh`；`{}` 被替换为匹配路径；`;` 需转义。

### `find`：在 Python 文件中找 TODO

```
missing:~$ find . -name "*.py" -exec grep -l "TODO" {} \;
```

**作用：** 组合 `find` 与 `grep -l`，定位含 TODO 的 `.py` 文件。

### `awk` 打印第二列

```
missing:~$ awk '{print $2}' file
```

**作用：** `awk` 最常见用途：按空白分列，打印第 2 列；加 `-F,` 可按逗号分列。

### 组合管道：从 SSH 日志提取高频用户名

```
missing:~$ ssh myserver 'journalctl -u sshd -b-1 | grep "Disconnected from"' \
  | sed -E 's/.*Disconnected from .* user (.*) [^ ]+ port.*/\1/' \
  | sort | uniq -c \
  | sort -nk1,1 | tail -n10 \
  | awk '{print $2}' | paste -sd,
postgres,mysql,oracle,dell,ubuntu,inspur,test,admin,user,root
```

**作用：** 展示管道如何把 `ssh`、`grep`、`sed`、`sort`、`uniq`、`awk`、`paste` 串成一条数据处理流水线。

### `for` 与命令替换 `$()`

```
for i in $(seq 1 10); do
```

**作用：** `$(seq 1 10)` 展开为 1..10，驱动 10 次循环；应优先用 `$()` 而非反引号。

### 复现 flaky 测试的脚本骨架

```
#!/bin/bash
set -euo pipefail

# Start CPU stress in background
stress --cpu 8 &
STRESS_PID=$!

# Setup log file
LOGFILE="test_runs_$(date +%s).log"
echo "Logging to $LOGFILE"

# Run tests until one fails
RUN=1
while cargo test my_test > "$LOGFILE" 2>&1; do
    echo "Run $RUN passed"
    ((RUN++))
done

# Cleanup and report
kill $STRESS_PID
echo "Test failed on run $RUN"
echo "Last 20 lines of output:"
tail -n 20 "$LOGFILE"
echo "Full log: $LOGFILE"
```

**作用：** 综合 shebang、`set -euo pipefail`、后台任务 `&`、`$!`、`while`、重定向 `> ... 2>&1`、命令替换与清理逻辑；用于在压力下复现不稳定测试。

## 练习要点

1. 确认使用 Unix Shell（Bash/ZSH）；Windows 用 WSL/VM；用 `echo $SHELL` 检查。
2. 用 `man ls` 理解 `ls -l` 与每行前 10 个字符（权限等）含义。
3. 理解 glob：实验 `*.txt`、`file?.txt`、`{a,b,c}.txt`。
4. 区分单引号、双引号、`$'ANSI quotes'`；写出含字面 `$`、`!`、换行的 echo。
5. 三个标准流 stdin/stdout/stderr；分别重定向；如何重定向到同一文件。
6. `$?`、`&&`、`||`；写「目录不存在才创建」的一行命令。
7. 思考为何 `cd` 必须是 Shell 内建（子进程无法改父进程工作目录）。
8. 写脚本：`$1` 为文件名，用 `test -f`/`[ -f ... ]` 判断是否存在并打印不同信息。
9. 保存为脚本后需 `chmod +x`；理解可执行权限。
10. 给 `set` 加 `-x`，观察跟踪执行输出。
11. 用 `$(date +%Y-%m-%d)` 做带日期的备份文件名。
12. 改 flaky 测试脚本：测试命令由参数 `$1`/`$@` 传入，而非写死 `cargo test`。
13. 用管道统计家目录下最常见的 5 种文件扩展名。
14. `find` + `xargs`（非 `-exec`）对 `.sh` 做 `wc -l`；处理含空格文件名（`-print0`/`-0`）。
15. `curl` 抓课程网站 HTML，用 `grep` 统计讲座数量。
16. `curl` + `jq`：从样例 JSON 提取 version > 6 的人名。
17. `awk`：第二列 > 100 的行，并交换第 1、3 列；用 `printf` 测例验证。
18. 拆解讲义中的 SSH 日志管道；仿照从 `~/.bash_history` / `~/.zsh_history` 找最常用命令。

## 延伸阅读

- Bash 手册：[Pattern Matching](https://www.gnu.org/software/bash/manual/html_node/Pattern-Matching.html)
- Bash 手册：[Quoting](https://www.gnu.org/software/bash/manual/html_node/Quoting.html)
- Bash 手册：[Redirections](https://www.gnu.org/software/bash/manual/html_node/Redirections.html)
- Bash 手册：[Exit Status](https://www.gnu.org/software/bash/manual/html_node/Exit-Status.html)
- Bash 手册：[Bash Conditional Expressions](https://www.gnu.org/software/bash/manual/html_node/Bash-Conditional-Expressions.html)
- Bash 手册：[Command Substitution](https://www.gnu.org/software/bash/manual/html_node/Command-Substitution.html)
- Bash 手册：[Special Parameters](https://www.gnu.org/software/bash/manual/html_node/Special-Parameters.html)
- Bash 手册：[The Set Builtin](https://www.gnu.org/software/bash/manual/html_node/The-Set-Builtin.html)
- `man xargs`；建议配合 `shellcheck` 写脚本
- 下一讲将深入 Shell 自动化与更多命令行程序（含 `ssh`）
