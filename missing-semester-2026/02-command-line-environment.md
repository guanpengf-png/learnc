# Command-line Environment

原讲义：https://missing.csail.mit.edu/2026/command-line-environment/

## 本讲目标

- 理解 CLI 程序如何通过**参数、流、环境变量、返回码、信号**与 Shell 协作。
- 掌握参数（`$1`/`$@`/`$#`）、flags、glob，以及 stdin/stdout/stderr 与并发管道。
- 学会环境变量与 `export`、引号差异、命令替换与进程替换、`&&`/`||`。
- 理解信号（SIGINT/SIGTERM/SIGKILL 等）、作业控制（Ctrl-Z、`fg`/`bg`、`jobs`、`nohup`、`disown`、`trap`）。
- 能用 SSH 密钥、`scp`/`rsync`、`~/.ssh/config` 连接远程机；用 `tmux` 管理会话/窗口/窗格。
- 会配置 dotfiles、aliases、包管理器；了解 Shell 中的 AI 工具与终端模拟器选择。

## 知识点

### CLI 约定总览

- Shell 脚本围绕「运行程序并让它们高效通信」设计；成功协作依赖约定。
- 五个核心概念：**Arguments、Streams、Environment variables、Return codes、Signals**。

### 参数（Arguments）

- 参数都是字符串；程序自行解释。例：`ls -l folder/` → 参数 `['-l', 'folder/']`。
- 脚本内：`$1`…`$9`、全部参数 `$@`、个数 `$#`、程序名 `$0`。
- Flags：`-` / `--` 开头，通常可选，改变行为；短选项可合并（`ls -la`）；顺序通常无关。
- 常见 flags：`--help`、`--verbose`、`--version`。
- 多数语言用 argparse 等库解析带 dash 的 CLI。
- 可变数量同类参数：`mkdir src docs` 等价于两次 `mkdir`。

### Glob（通配）

- Shell **在调用程序前**展开 glob；`rm *.py` 实际传入匹配到的文件名列表，而非字面 `*.py`。
- 常见：`*`（任意长度）、`?`（恰好一个字符）、`{a,b,c}`（花括号展开为多个参数）。
- 可组合：`mv *{.py,.sh} folder`。
- 部分 Shell（如 zsh）支持 `**` 递归 glob。

### 流（Streams）与管道并发

- `|` 连接的是**数据流**；管道中各程序**同时启动**，不是串行跑完再启动下一个。
- 每个程序：stdin（标准输入）、stdout（标准输出，用于管道）、stderr（标准错误，通常不进管道）。
- 许多程序用文件名 `-` 表示「从 stdin 读」。
- 重定向：`>`、`>>`、`2>`、`&>`、`<`、丢弃到 `/dev/null` 等。
- `fzf`：从 stdin 读行，交互模糊筛选（可接 `ls`、history）。

### 环境变量与引号

- 赋值：`foo=bar`（`=` 两侧不能有空格，否则被当成程序名与参数）。
- 取值：`$foo`；Shell 变量皆为字符串。
- 单引号：字面量，不展开变量/命令替换/转义；双引号：会展开。
- 命令替换：`files=$(ls)` 捕获 stdout。
- 进程替换：`<(CMD)` 把命令输出放到临时文件，并把路径替换进去（适合要「文件参数」的命令，如 `diff`）。
- `printenv` 查看环境；`VAR=value cmd` 只影响该子进程；`export VAR=value` 影响当前 Shell 及后续子进程；`unset` 删除。
- 约定：环境变量常用 `ALL_CAPS`（如 `HOME`、`PATH`、`DEBUG`、`TZ`）。

### 返回码

- 约定：0 成功，非 0 失败；`exit NUM` 设置退出码；`$?` 为上一命令退出码。
- `&&` / `||` 按返回码短路：成功才跑右边 / 失败才跑右边。
- `if` / `while` 也基于返回码（0 为真）。
- `true` 总成功；`false` 总失败。

### 信号与作业控制

- Ctrl-C → Shell 向前台进程发 **SIGINT**。
- Ctrl-`\` → 常发 **SIGQUIT**。
- **SIGTERM**：优雅请求退出（`kill -TERM <pid>`）。
- Ctrl-Z → **SIGTSTP**（终端停止）；之后可用 `fg` / `bg` 继续。
- **SIGSTOP** 暂停；**SIGKILL** 不可捕获、立即杀掉（可能留下孤儿进程）。
- `jobs` 列本终端作业；可用 `%N` 或 `$!`（最近后台作业）引用。
- 命令后缀 `&`：后台运行；关闭终端会发 **SIGHUP** 导致子进程退出。
- `nohup`：忽略 SIGHUP；`disown`：已启动进程脱离终端；或用终端复用器。
- `trap`：在收到信号或 EXIT 时执行清理函数。

### 远程机器（SSH）

- `ssh user@host`；可非交互跑远程命令；注意引号决定管道在本地还是远程执行。
- 认证：密码或密钥；密钥更安全方便；私钥（如 `~/.ssh/id_ed25519`）等同密码，勿泄露。
- `ssh-keygen` 生成密钥；公钥放入远程 `~/.ssh/authorized_keys`（`ssh-copy-id`）。
- `scp`：安全拷文件；`rsync`：增量、权限/符号链接控制、可 `--partial` 断点续传。
- `~/.ssh/config`：Host 别名、User、HostName、Port、IdentityFile；也可通配；`scp`/`rsync`/`mosh` 等也会读。
- 建议了解 **Mosh**：更耐断线、休眠、换网络、高延迟。

### tmux（终端复用器）

- 层次：**Sessions → Windows → Panes**。
- 前缀键：`Ctrl-b` 再按命令键。
- 会话：新建、命名、列表、分离（`d`）、附着（`tmux a`）。
- 窗口：新建 `c`、切编号、前后、重命名、列表。
- 窗格：水平/垂直分割、方向切换、缩放、滚回与复制、布局循环。
- 远程工作时可避免依赖 `nohup` 等技巧。

### Dotfiles、别名、包管理器

- Dotfiles：以 `.` 开头的配置文件（默认 `ls` 隐藏），如 `~/.bashrc`、`~/.gitconfig`、`~/.vimrc`、`~/.ssh/config`、`~/.tmux.conf`。
- 扩展 `PATH`：`export PATH="$PATH:path/to/append"`。
- 包管理器：Homebrew（macOS）、`apt`、`dnf`、`pacman`；例：`brew install ripgrep fd`。
- 警惕 `curl | bash`：先下载、`less` 审查再执行更安全。
- `command-not-found.com`：查如何安装某命令；`tldr`：看常见用法。
- `alias`：命令简写；`=` 旁无空格；可用 `\cmd` 临时忽略别名；`unalias` 取消。
- 别名不能在命令中间插参数 → 复杂逻辑用函数。
- Ctrl-R：历史反向搜索；配 `fzf` 后可模糊搜历史。
- Dotfiles 建议：独立目录 + 版本控制 + 脚本 symlink；便于新机器一分钟装好、可移植、可同步、可追踪。
- Shell 框架/插件：prezto、oh-my-zsh；或更轻量的 zsh-syntax-highlighting、zsh-autosuggestions、powerlevel10k 等；大框架可能拖慢启动。

### AI in the Shell / 终端模拟器

- `llm cmd "..."`：自然语言生成命令；也可把 LLM 接入管道处理不规则文本。
- AI shells（如 Claude Code）：用英语驱动 shell 操作与多步任务。
- 终端模拟器可调：字体、配色、快捷键、Tab/Pane、scrollback、性能（如 Alacritty、Ghostty 的 GPU 加速）。

## 代码/命令示例

### 对比「普通函数」与「Shell 脚本接口」

```
def add(x: int, y: int) -> int:
    return x + y
```

```
#!/usr/bin/env bash

if [[ -f $1 ]]; then
    echo "Target file already exists"
    exit 1
else
    if $DEBUG; then
        grep 'error' - | tee $1
    else
        grep 'error' - > $1
    fi
    exit 0
fi
```

**作用：** 说明 Shell 程序通过参数（`$1`）、环境变量（`$DEBUG`）、stdin（`grep ... -`）、重定向/`tee`、退出码与外界交互，而不像普通函数那样显式类型化输入输出。

### 可变参数与 glob 替代循环删除

```
mkdir src
mkdir docs
# is equivalent to
mkdir src docs
```

```
for file in $(ls | grep -P '\.py$'); do
    rm "$file"
done
```

```
rm *.py
```

**作用：** 展示「多参数一次调用」约定，以及 glob 如何在调用前展开，从而用一行替代脆弱的 `ls|grep` 循环。

### 花括号与组合 glob

```
touch folder/{a,b,c}.py
# Will expand to
touch folder/a.py folder/b.py folder/c.py

convert image.{png,jpg}
# Will expand to
convert image.png image.jpg

cp /path/to/project/{setup,build,deploy}.sh /newpath
# Will expand to
cp /path/to/project/setup.sh /path/to/project/build.sh /path/to/project/deploy.sh /newpath

# Globbing techniques can also be combined
mv *{.py,.sh} folder
# Will move all *.py and *.sh files
```

**作用：** 用具体展开结果讲清 `{}`、以及与 `*` 组合的写法。

### 管道中进程并发

```
$ (sleep 15 && cat numbers.txt) | grep -P '^\d$' | sort | uniq  &
[1] 12345
$ ps | grep -P '(sleep|cat|grep|sort|uniq)'
  32930 pts/1    00:00:00 sleep
  32931 pts/1    00:00:00 grep
  32932 pts/1    00:00:00 sort
  32933 pts/1    00:00:00 uniq
  32948 pts/1    00:00:00 grep
```

**作用：** 证明管道里各进程几乎立刻被创建并连上流；`cat` 要等 `sleep` 结束才真正开始产出。

### stdin 的 `-` 约定

```
# These are equivalent when data comes from a pipe
echo "hello" | grep "hello"
echo "hello" | grep "hello" -
```

**作用：** 说明许多程序把 `-` 当作「从标准输入读取」。

### stderr 不进管道与重定向

```
$ ls /nonexistent
ls: cannot access '/nonexistent': No such file or directory
$ ls /nonexistent | grep "pattern"
ls: cannot access '/nonexistent': No such file or directory
# The error message still appears because stderr is not piped
$ ls /nonexistent 2>/dev/null
# No output - stderr was redirected to /dev/null
```

**作用：** 区分 stdout（可被 `|`）与 stderr（默认仍打到终端）。

### 常用重定向写法

```
# Redirect stdout to a file (overwrite)
echo "hello" > output.txt

# Redirect stdout to a file (append)
echo "world" >> output.txt

# Redirect stderr to a file
ls foobar 2> errors.txt

# Redirect both stdout and stderr to the same file
ls foobar &> all_output.txt

# Redirect stdin from a file
grep "pattern" < input.txt

# Discard output by redirecting to /dev/null
cmd > /dev/null 2>&1
```

**作用：** 汇总 stdout/stderr/stdin 重定向与丢弃输出的标准写法。

### `fzf` 模糊选择

```
$ ls | fzf
$ cat ~/.bash_history | fzf
```

**作用：** 展示从 stdin 读行并交互筛选的 Unix 风格工具。

### 单双引号与命令/进程替换

```
foo=bar
echo "$foo"
# prints bar
echo '$foo'
# prints $foo
```

```
files=$(ls)
echo "$files" | grep README
echo "$files" | grep ".py"
```

```
diff <(ls src) <(ls docs)
```

**作用：** 对比引号展开规则；命令替换捕获输出；进程替换把命令输出当「文件」传给 `diff`。

### 临时环境变量 vs `export`

```
TZ=Asia/Tokyo date  # prints the current time in Tokyo
echo $TZ  # this will be empty, since TZ was only set for the child command
```

```
export DEBUG=1
# All programs from this point onwards will have DEBUG=1 in their environment
bash -c 'echo $DEBUG'
# prints 1
```

**作用：** 说明「前缀赋值只影响一次子进程」与「export 写入当前环境并遗传」的区别。

### `&&` / `||` 与基于返回码的控制流

```
# echo will only run if grep succeeds (finds a match)
grep -q "pattern" file.txt && echo "Pattern found"

# echo will only run if grep fails (no match)
grep -q "pattern" file.txt || echo "Pattern not found"

# true is a shell program that always succeeds
true && echo "This will always print"

# and false is a shell program that always fails
false || echo "This will always print"
```

```
# if uses the return code of the condition command (0 = true, nonzero = false)
if grep -q "pattern" file.txt; then
    echo "Found"
fi

# while loops continue as long as the command returns 0
while read line; do
    echo "$line"
done < file.txt
```

**作用：** 把返回码约定落到 `&&`/`||`/`if`/`while` 的实际写法上。

### 捕获 SIGINT 的 Python 示例

```
#!/usr/bin/env python
import signal, time

def handler(signum, time):
    print("\nI got a SIGINT, but I am not stopping")

signal.signal(signal.SIGINT, handler)
i = 0
while True:
    time.sleep(.1)
    print("\r{}".format(i), end="")
    i += 1
```

**作用：** 说明进程可捕获 SIGINT（Ctrl-C）并忽略；此时可用 Ctrl-`\`（SIGQUIT）等结束。

### 作业控制会话示例

```
$ sleep 1000
^Z
[1]  + 18653 suspended  sleep 1000

$ nohup sleep 2000 &
[2] 18745
appending output to nohup.out

$ jobs
[1]  + suspended  sleep 1000
[2]  - running    nohup sleep 2000

$ kill -SIGHUP %1
[1]  + 18653 hangup     sleep 1000

$ kill -SIGHUP %2   # nohup protects from SIGHUP

$ jobs
[2]  + running    nohup sleep 2000

$ kill %2
[2]  + 18745 terminated  nohup sleep 2000
```

**作用：** 串联 Ctrl-Z、`nohup`、`jobs`、`%N`、`SIGHUP` 与普通 `kill` 的行为差异。

### `trap` 清理

```
#!/usr/bin/env bash
cleanup() {
    echo "Cleaning up temporary files..."
    rm -f /tmp/mytemp.*
}
trap cleanup EXIT  # Run cleanup when script exits
trap cleanup SIGINT SIGTERM  # Also on Ctrl-C or kill
```

**作用：** 在脚本退出或收到 SIGINT/SIGTERM 时统一做清理。

### SSH 本地/远程管道与密钥

```
ssh alice@server.mit.edu
```

```
# here ls runs in the remote, and wc runs locally
ssh alice@server ls | wc -l

# here both ls and wc run in the server
ssh alice@server 'ls | wc -l'
```

```
ssh-keygen -a 100 -t ed25519 -f ~/.ssh/id_ed25519
```

```
cat .ssh/id_ed25519.pub | ssh alice@remote 'cat >> ~/.ssh/authorized_keys'

# or more simply (if ssh-copy-id is available)

ssh-copy-id -i .ssh/id_ed25519 alice@remote
```

**作用：** 建立 SSH 连接；理解引号如何决定管道执行位置；生成并部署 ed25519 公钥。

### `~/.ssh/config` 示例

```
Host vm
    User alice
    HostName 172.16.174.141
    Port 2222
    IdentityFile ~/.ssh/id_ed25519

# Configs can also take wildcards
Host *.mit.edu
    User alice
```

**作用：** 用 Host 别名与通配简化 `ssh`/`scp`/`rsync` 连接参数。

### 扩展 PATH、安全安装、`tldr`、aliases

```
export PATH="$PATH:path/to/append"
```

```
curl -fsSL https://example.com/install.sh -o install.sh
less install.sh # review the script
bash install.sh
```

```
$ tldr fd
  An alternative to find.
  Aims to be faster and easier to use than find.

  Recursively find files matching a pattern in the current directory:
      fd "pattern"

  Find files that begin with "foo":
      fd "^foo"

  Find files with a specific extension:
      fd --extension txt
```

```
alias alias_name="command_to_alias arg1 arg2"
```

```
# Make shorthands for common flags
alias ll="ls -lh"

# Save a lot of typing for common commands
alias gs="git status"
alias gc="git commit"

# Save you from mistyping
alias sl=ls

# Overwrite existing commands for better defaults
alias mv="mv -i"           # -i prompts before overwrite
alias mkdir="mkdir -p"     # -p make parent dirs as needed
alias df="df -h"           # -h prints human readable format

# Alias can be composed
alias la="ls -A"
alias lla="la -l"

# To ignore an alias run it prepended with \
\ls
# Or disable an alias altogether with unalias
unalias la

# To get an alias definition just call it with alias
alias ll
# Will print ll='ls -lh'
```

**作用：** 覆盖 PATH 扩展、审查后再执行安装脚本、用 `tldr` 学命令、以及 alias 的写法/组合/忽略。

### Homebrew 安装现代工具

```
# ripgrep: a faster grep with better defaults
brew install ripgrep

# fd: a faster, user-friendly find
brew install fd
```

**作用：** 示范用包管理器安装讲义推荐的 `rg`/`fd`。

### AI：命令生成与管道抽取

```
$ llm cmd "find all python files modified in the last week"
find . -name "*.py" -mtime -7
```

```
$ INSTRUCTIONS="Extract just the username from each line, one per line, nothing else"
$ llm "$INSTRUCTIONS" < users.txt
```

**作用：** 展示 `simonw/llm` 生成命令，以及把不规则文本经 stdin 交给 LLM 抽取结构化结果（含引号与重定向要点）。

## 练习要点

### Arguments and Globs

1. 理解 `--`：停止解析 flags；练习 `touch -- -myfile` 及如何删除以 `-` 开头的文件。
2. 写 `ls`：含隐藏文件、人类可读大小、按时间、彩色输出。
3. 用进程替换比较 `printenv` 与 `export` 的差异。

### Environment Variables

4. 实现 `marco`/`polo`：记住目录并随时 `cd` 回去；用 `source` 加载。

### Return Codes

5. 循环跑「偶发失败」脚本直到失败，捕获 stdout/stderr，报告运行次数。

### Signals and Job Control

6. `sleep 10000` → Ctrl-Z → `bg` → 用 `pgrep`/`pkill`（勿手打 pid）杀掉。
7. `wait` 等子进程；跨会话时用 `kill -0` + `sleep` 写 `pidwait`。

### Files and Permissions

8. （进阶）递归找目录中最近修改的文件 / 按新旧列出。

### Terminal Multiplexers

9. 跟做 tmux 教程并做基础自定义。

### Aliases and Dotfiles

10–17. 别名 `dc=cd`；从 history 找 Top 10 命令并起短别名；建版本控制的 dotfiles 仓；至少配一个程序；写安装/symlink 脚本；在新 VM 测试；迁移配置并发布到 GitHub。

### Remote Machines (SSH)

18–24. 生成 ed25519 密钥；写 `~/.ssh/config`（含 `LocalForward`）；`ssh-copy-id`；本机访问 VM 上的 Python HTTP server；加固 `sshd_config`；挑战 Mosh 断网恢复与 `ssh -N -f` 后台端口转发。

## 延伸阅读

- 信号：`man signal`、`kill -l`；讲义中的 signals 链接
- tmux：[quick tutorial](https://www.hamvocke.com/blog/a-quick-and-easy-guide-to-tmux/)、更详细说明
- Shell 启动文件加载顺序相关资源（讲义指向的 excellent resource）
- [command-not-found.com](https://command-not-found.com/)
- Dotfiles：GitHub 热门 dotfiles、讲义作者 Anish / Jon / Jose 的公开配置
- zsh 插件：zsh-syntax-highlighting、zsh-autosuggestions、zsh-completions、zsh-history-substring-search、powerlevel10k；框架 prezto / oh-my-zsh
- AI：`simonw/llm`；终端：Alacritty、Ghostty 等
- Mosh：作为更耐断线的 SSH 替代
