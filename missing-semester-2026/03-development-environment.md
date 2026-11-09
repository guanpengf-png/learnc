# Development Environment and Tools

原讲义：https://missing.csail.mit.edu/2026/development-environment/

## 本讲目标

- 理解开发环境构成：文本编辑 + 语法高亮、类型检查、格式化、补全等；对比 **IDE** 与 **终端工作流**。
- 掌握 Vim 的模态编辑思想，以及 movement（名词）/ edit verbs（动词）/ counts / modifiers 的组合。
- 能跟做讲义中的 **fizzbuzz 修复** 按键流程，体会「用 Vim 语言改代码」。
- 了解 Language Server Protocol（LSP）提供的代码智能功能，以及如何配置语言服务器。
- 认识 AI 编程的三种形态：autocomplete、inline chat、coding agents。
- 了解常用 IDE 扩展：devcontainers、Remote SSH、Live Share。

## 知识点

### IDE vs 终端工作流

- 开发环境核心是文本编辑，以及语法高亮、类型检查、格式化、自动补全等配套能力。
- **IDE**（如 VS Code）：把功能集中在一个应用里；通常更易上手，开箱 AI 集成更好。
- **终端工作流**：tmux + Vim + Zsh + 语言工具（如 Ruff、Mypy）等组合；轻量，无 GUI / 难装软件时往往是唯一选择。
- 建议：两边都有基本熟悉，并至少精通一种；若无偏好 IDE，可从 VS Code 开始。

### Vim 哲学与适用范围

- 编程大量时间是**导航、阅读片段、局部编辑**，而非从头写长文或通读文件；Vim 为此优化。
- Vim 接口本身像一门语言：带助记名的按键是命令，且**可组合**；避免鼠标，甚至尽量少用方向键。
- 不必只用 Vim 本体：许多工具支持 Vim mode（VSCodeVim、Zsh Vim 模拟、Claude Code 的 Vim 编辑模式等）。

### 模态编辑（Modal editing）

| 模式 | 用途 | 进入方式（从 Normal） |
|------|------|----------------------|
| Normal | 移动与编辑（默认） | Esc（从任意模式回 Normal） |
| Insert | 插入文本 | `i` |
| Replace | 替换文本 | `R` |
| Visual / Visual Line / Visual Block | 选择文本 | `v` / `V` / Ctrl-V（`^V`） |
| Command-line | 运行命令 | `:` |

- 同一按键在不同模式含义不同：如 `x` 在 Insert 插入字符，在 Normal 删除光标下字符，在 Visual 删除选区。
- 默认左下角显示当前模式；多数时间在 Normal ↔ Insert。
- Esc 用得多：可考虑把 Caps Lock 映射为 Escape，或设替代按键序列。

### Vim 接口即编程语言

#### Movement（名词 / nouns）

- 基本：`h j k l`（左下上右）
- 词：`w` 下一词，`b` 词首，`e` 词尾
- 行：`0` 行首，`^` 首个非空白，`$` 行尾
- 屏幕：`H` / `M` / `L`（屏顶/中/底）；滚动 `Ctrl-u` / `Ctrl-d`
- 文件：`gg` 文件头，`G` 文件尾；`:{number}` 或 `{number}G` 跳行
- `%`：匹配括号等；行内查找 `f/t/F/T{char}`，`,`/`;` 跳匹配
- 搜索：`/{regex}`，`n`/`N` 下一/上一匹配

#### Selection

- Visual / Visual Line / Visual Block；再用 movement 扩展选区。

#### Edits（动词 / verbs）

- `i` 进 Insert；`o`/`O` 在下/上方开新行
- `d{motion}` 删除；`c{motion}` 更改（删后进 Insert）；`x`≈`dl`；`s`≈`cl`
- Visual 选区后 `d`/`c`；`u` 撤销，Ctrl-R 重做；`y` yank（复制），`p` paste
- 其他：`~` 切换大小写，`J` 合并行等

#### Counts

- 数字前缀重复动作：`3w`、`5j`、`7dw`

#### Modifiers

- `i` = inner（内部），`a` = around（含边界）
- 例：`ci(`、`ci[`、`da'`

### 学习 Vim

- 先掌握基础，再在所有支持 Vim mode 的软件里坚持使用；避免鼠标/方向键（可解绑方向键逼自己练）。
- 额外资源：往期更深的 Vim 讲座、`vimtutor`、Vim Adventures、Vim Tips Wiki、Vim Advent Calendar、VimGolf、Vi/Vim Stack Exchange、Vim Screencasts、书 *Practical Vim*。

### 代码智能与 Language Servers

- IDE 通过扩展连接实现 **Language Server Protocol** 的语言服务器（如 VS Code Python → Pylance，Go → gopls）。
- 常见能力：
  - Code completion（如 `object.` 后字段/方法）
  - Inline documentation（悬停/建议中的文档）
  - Jump-to-definition
  - Find references
  - Help with imports（整理/删未用/标缺失）
  - Code quality：格式化、类型检查、lint（也可独立使用；代码质量讲座会更深入）
- 配置：有的装扩展即可；有的需指向运行环境（如 Python 解释器）才能看到已装包；还可按项目开关静态类型检查等设置。

### AI-powered development

三种主流形态：

1. **Autocomplete**：在光标处补全；可用注释引导；局限是通常只能在光标后补全（如可能把 `import` 补进函数内）。
2. **Inline chat**：选中代码后用自然语言请求修改；可改已有代码（不同于仅往后补全）。
3. **Coding agents**：本课在 *Agentic Coding* 讲座深入。

推荐软件示例：VS Code + GitHub Copilot、Cursor；学生/教师/热门开源维护者可能免费用 Copilot；领域变化快，头部产品功能大体相当。

### 扩展与其他 IDE 功能

- **Development containers**：用容器跑开发工具，利于可移植与隔离（packaging 讲座更深入）。
- **Remote development**：如 VS Code Remote SSH，在云端 GPU 机上开发运行。
- **Collaborative editing**：如 Live Share，类 Google Docs 同编。
- 鼓励自行探索：Vim Awesome、VS Code 扩展热门列表等。

## 代码/命令示例

### 进入 Insert 与回到 Normal（概念操作）

```
i          # Normal → Insert
<Esc>      # 任意模式 → Normal
R / v / V / Ctrl-v / :   # 分别进入 Replace / Visual / Visual Line / Visual Block / Command-line
```

**作用：** 建立模态编辑的基本状态机；后续高效编辑都建立在「多数时间 Normal，需要打字才 Insert」。

### Movement / verb / count / modifier 组合示例

```
hjkl          # 基本移动
w b e         # 按词移动
0 ^ $         # 行首 / 首非空白 / 行尾
gg G 42G      # 文件头 / 尾 / 第 42 行
dw d$ d0      # 删词 / 删到行尾 / 删到行首
cw            # 改词（delete word + insert）
3w 5j 7dw     # 带 count
ci( ci[ da'   # 带 modifier：改括号内、改方括号内、删含引号的单引号字符串
```

**作用：** 体现 Vim「名词 + 动词 + 次数 + 修饰」可组合的语言接口。

### 有问题的 fizzbuzz（修复前）

```
def fizz_buzz(limit):
    for i in range(limit):
        if i % 3 == 0:
            print("fizz", end="")
        if i % 5 == 0:
            print("fizz", end="")
        if i % 3 and i % 5:
            print(i, end="")
        print()

def main():
    fizz_buzz(20)
```

**作用：** 作为实操靶子：缺 `__main__` 调用、从 0 起算、5 的倍数误打印 `"fizz"`。

### Fizzbuzz 修复按键流程（从 Normal 开始）

**1）调用 main**

```
G
o
if __name__ == "__main__": main()
<Esc>
```

**作用：** `G` 到文件末尾，`o` 下方开新行并进入 Insert，补上入口调用。

**2）range 从 1 开始且上界正确**

```
/range<CR>
ww
i
1,
<Esc>
e
a
+ 1
<Esc>
```

**作用：** 搜索 `range`，移到参数处插入 `1,`，再在末尾追加 `+ 1`，把 `range(limit)` 改成 `range(1, limit + 1)`。小 count 时实务上常重复按键而非写数字。

**3）5 的倍数应打印 buzz**

```
:6<CR>
ci"
buzz
<Esc>
```

**作用：** 跳到第 6 行，用 `ci"` 改引号内内容为 `"buzz"`，展示 modifier + 文本对象。

### AI autocomplete：由签名补全函数体

```
import requests

def download_contents(url: str) -> str:
```

模型可能补全为：

```
    response = requests.get(url)
    return response.text
```

**作用：** 展示光标处被动补全「刚刚够用」的实现。

### 用注释引导更好的补全

差名字时模型可能补成「非空行」：

```
def extract(contents: str) -> list[str]:
```

```
    lines = contents.splitlines()
    return [line for line in lines if line.strip()]
```

加注释后：

```
def extract(content: str) -> list[str]:
    # extract all Markdown links from the content
```

```
    import re
    pattern = r'\[.*?\]\((.*?)\)'
    return re.findall(pattern, content)
```

**作用：** 说明注释可引导补全；也暴露局限——`import re` 可能被补在函数内，而更好做法是模块级导入。实务上应用描述性函数名与 docstring。

### 演示用完整脚本收尾

```
print(extract(download_contents("https://raw.githubusercontent.com/missing-semester/missing-semester/refs/heads/master/_2026/development-environment.md")))
```

**作用：** 把 download + extract 串成可运行示例（抓取本讲 Markdown 并抽链接）。

### Inline chat：改用标准库

选中相关代码后提示：

```
use built-in libraries instead
```

模型可改为：

```
from urllib.request import urlopen

def download_contents(url: str) -> str:
    with urlopen(url) as response:
        return response.read().decode('utf-8')
```

**作用：** 对比 autocomplete：inline chat 能改写已有代码块，而不只是在光标后追加。

## 练习要点

1. 在所有支持 Vim mode 的软件（编辑器、Shell 等）启用并坚持约一个月；觉得别扭就搜「更好做法」。
2. 完成一道 [VimGolf](https://www.vimgolf.com/) 挑战。
3. 为正在做的项目（或 GitHub 开源项目，含本课仓库）配置 IDE 扩展与语言服务器，确认 jump-to-definition（含库依赖）等可用。
4. 浏览扩展列表，安装一个觉得有用的扩展。

## 延伸阅读

- 往期 Missing Semester 的 Vim 深度讲座
- 系统自带：`vimtutor`
- [Vim Adventures](https://vim-adventures.com/)、Vim Tips Wiki、Vim Advent Calendar、VimGolf、Vi/Vim Stack Exchange、Vim Screencasts
- 书籍：*Practical Vim*
- 插件/扩展列表：Vim Awesome；VS Code 扩展按热度排序
- 后续相关讲座：Code Quality（格式化/lint/类型检查更深入）、Packaging and shipping code（环境与容器）、Agentic Coding（编码代理）
