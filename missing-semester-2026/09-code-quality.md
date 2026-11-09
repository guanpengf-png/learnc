# Code Quality

原讲义：https://missing.csail.mit.edu/2026/code-quality/

## 本讲目标

- 掌握提升代码质量的工具链概念：Formatting、Linting、Testing、Pre-commit、CI/CD、Command runners。
- 理解语言特定工具（如 Python 的 Ruff）与跨语言工具（如 Prettier、semgrep、EditorConfig）的共性。
- 学会测试层次、覆盖率、属性测试（Hypothesis）、mock；CI（如 GitHub Actions）与持续部署的用途。
- 掌握正则基础语法、捕获组、局限；能用 `re.match` 抽取日期中的月份，并理解为何复杂解析应改用真正的解析器。

## 知识点

### 总览

本讲覆盖：

- Formatting
- Linting
- Testing
- Pre-commit hooks
- Continuous integration
- Command runners

Bonus：**正则表达式**——横切主题（如按模式跑子集测试、IDE 搜索替换等）。

- 许多工具语言相关（Ruff）；有的跨语言（Prettier）。概念近乎普适：几乎每种语言都有 formatter、linter、测试库等。

### Formatting

- 自动美化表面语法：引号风格、运算符周围空格、`import` 排序、行长等——让人专注更深问题。
- 重大收益：在团队中**标准化风格**。
- Prettier 等高度可配置 → 配置文件应入库。
- Black、gofmt 等几乎不可配置 → **减少 bikeshedding**。
- IDE 集成：输入时或保存时自动格式化。
- **EditorConfig**：项目级约定（如各文件类型缩进），与 IDE 沟通。

### Linting

- **静态分析**（不运行代码）找反模式与潜在问题；比 formatter 更深，深度因工具而异。
- 规则列表 + 项目级预设；误报可按文件/行禁用。
- 好的 linter 对每条规则有文档：查什么、为何不好、更好写法。例：Ruff 的 **SIM102**（不必要嵌套 `if`）。
- 部分规则可自动修复。
- **semgrep**：「语义 grep」，在 AST 层工作（非字符级），多语言；便于写自定义规则。例：禁止危险的 `subprocess.Popen(..., shell=True)`。

### Testing

- 写代码 + 写行使该代码并在不符预期时报错的代码 → 提升正确性信心。
- 粒度：
  - **Unit**：单个函数
  - **Integration**：模块/服务交互
  - **Functional**：端到端场景
- **TDD**：实现前先写测试。
- **Regression tests**：修 bug 后留下，防止再破。
- **Property-based tests**：源自 Haskell QuickCheck；Python 有 **Hypothesis** 等。
- 实际常组合多种方式。
- 外部依赖（DB、Web API）：测试中 **mock**，避免测时直连第三方。

#### Code coverage

- 看测试运行时执行了哪些行，确保覆盖路径；可逐行展示指导写测。
- Codecov 等提供历史覆盖的 Web 界面。
- 任何指标都不完美：**勿过度盯覆盖率**，重点写高质量测试。

### Pre-commit hooks

- Git pre-commit：每次 commit 前自动跑用户指定代码。
- **pre-commit** 框架让配置更容易。
- 常见：跑 formatter、linter，有时也跑测试 → 确保提交符合风格且无特定问题。

### Continuous integration（CI）

- 如 **GitHub Actions**：每次 push、每个 PR、或按计划跑脚本。
- 常见：formatter、linter、测试；编译语言确保可编译；静态类型语言做类型检查。
- Push：抓主线引入的错误；PR：抓贡献者提交问题；Schedule：抓外部依赖破坏（如误标 semver 兼容的 breaking change）。
- CI 适合长时间任务；可做 **matrix**（多 OS、多语言版本）。
- 一般用 **check-only** 而非 fix 模式（如格式不符则报错，不直接改仓库）。
- README 常挂 status badge（CI、覆盖率等）。Missing Semester 的链接检查（proof-html Action）常因第三方站点失败，但仍抓到许多坏链。
- 学习捷径：找高质量同类开源项目，研究 `pyproject.toml`、`.github/workflows/`、`DEVELOPMENT.md` 等。

#### Continuous deployment（CD）

- 用 CI 基础设施真正部署。例：Missing Semester 推送讲义后自动构建部署到 GitHub Pages。
- 也可构建二进制、Docker 镜像等产物。

### Command runners

- **just** 等简化项目上下文中的命令：不必记 `uv run ruff check --fix`，可变成 `just lint`，以及 `just format`、`just typecheck` 等。
- 语言特定工具也可能内置：npm `package.json` 的 `scripts`；Hatch 的 `tool.hatch.envs.*.scripts`（`pyproject.toml`）。

### Regular expressions（正则）

- 表示字符串集合的语言；用于 CLI、IDE、语言内置/`re` 库等。
- 例：`ag "import .* as .*"` 找 Python 重命名 import；`go test -run [regexp]` 选测试子集。
- 本讲用 **Python regex 语法**；不同 flavor 在高级功能上有差异；可用 regex101 调试。

#### 直觉示例

| 模式 | 含义 |
|------|------|
| `abc` | 字面 “abc” |
| `missing\|semester` | “missing” 或 “semester” |
| `\d{4}-\d{2}-\d{2}` | YYYY-MM-DD 形如 “2026-01-14”；**不**校验真实日期，“2026-01-99” 也匹配 |
| `.+@.+` | 极简「邮箱」；会匹配 “nonsense@@@email”；无假阳性/假阴性的完整邮箱正则存在但不实用 |

#### 语法积木

- `abc`：字面（无特殊含义时）
- `.`：任意单字符
- `[abc]` / `[^abc]`：字符类 / 否定
- `[a-f]`：范围
- `a|b`：二选一
- `\d` 数字；`\w` 单词字符；`\b` 词边界
- `(...)`：分组
- `...?` 零或一（如 `words?`）；`...*` 任意次；`...+` 一次及以上；`...{N}` 恰 N 次
- `\.` 字面点；`\\` 字面反斜杠
- `^` / `$`：行首 / 行尾

#### Capture groups

- 分组可用于提取或搜索替换。
- 编辑器替换引用语法因 IDE 而异：VS Code `$1`、`$2`；Vim `\1`、`\2`。

#### Limitations

- 正则语言能力有限：无法表达 {aⁿbⁿ | n ≥ 0}；HTML 等也不是正则语言。
- 现代引擎有 lookahead、backreferences 等，实用上很强，但表达力仍有限。
- 更复杂语言需更强解析器（如 **pyparsing** PEG）。
- 学习建议：掌握本讲基础，需要时查手册，不必背完整语言。
- LLM 可帮生成正则（讲义给了 Nginx 日志提取 path 的 prompt 示例）。

## 代码/命令示例

### semgrep 找危险 `shell=True`

```bash
semgrep -l python -e "subprocess.Popen(..., shell=True, ...)"
```

**作用：** 在 AST 层匹配危险调用模式，便于自定义 lint 规则；比纯字符 grep 更稳。

### 命令运行器简化调用（讲义对比）

```bash
uv run ruff check --fix
just lint
```

**作用：** 展示用 `just`（或同类）把长命令收成项目内统一短命令。

### 用捕获组提取 YYYY-MM-DD 中的月份

```python
>>> import re
>>> re.match(r"\d{4}-(\d{2})-\d{2}", "2026-01-14").group(1)
'01'
```

**作用：** 演示 `(...)` 捕获与 `.group(1)`；从日期字符串只取出月份。

### 日期形态匹配（不校验合法性）

```
\d{4}-\d{2}-\d{2}
```

**作用：** 匹配「四位-两位-两位」形态；提醒 “2026-01-99” 也会匹配。

### 让 LLM 生成 Nginx 日志 path 正则（讲义 prompt）

```
Write a Python-style regex pattern that matches the requested path from log lines from Nginx. Here is an example log line:

169.254.1.1 - - [09/Jan/2026:21:28:51 +0000] "GET /feed.xml HTTP/2.0" 200 2995 "-" "python-requests/2.32.3"
```

**作用：** 练习用对话式 AI 辅助写正则；对照真实日志样例迭代。

## 练习要点

1. 为正在做的项目配置 formatter、linter、pre-commit。格式问题交给自动格式化；linter 错误可用 AI agent 修，但须让 agent **能跑 linter 并看结果**以迭代；仔细检查别把代码修坏。
2. 学一门语言的测试库并写单元测试；跑覆盖率并生成 HTML 报告；手动补测；再用 agent 提覆盖（须能跑带覆盖的测试并看到逐行报告）——审视 AI 测试是否真好。
3. 设置每次 push 的 CI：跑 format、lint、tests；故意弄坏（如引入 linter 违规），确认 CI 抓住。
4. 用 `grep` 写正则找 `subprocess.Popen(..., shell=True)`，再尝试「弄破」该正则；看 semgrep 是否仍能匹配骗过 grep 的危险代码。
5. 在 IDE 里用正则把讲义中的 Markdown `-` 列表标记换成 `*`——不能全局替换所有 `-`（很多 `-` 不是列表符）。
6. 写正则从 `{"name": "Alyssa P. Hacker", "college": "MIT"}` 抽出 name；注意贪心量词陷阱（可能吃到 `"Alyssa P. Hacker", "college": "MIT`）。进阶：(1) 支持名字里含转义 `\"`；(2) **不要**用正则做复杂 JSON——用语言自带 JSON 解析器写 stdin→stdout 抽 name 的小程序（Python 除 `import json` 外一行即可）。

## 延伸阅读

- 本课：https://missing.csail.mit.edu/2026/code-quality/
- Ruff SIM102 文档（嵌套 if 规则示例）
- semgrep；Black / Prettier / gofmt；EditorConfig
- Hypothesis（属性测试）；Codecov；pre-commit 框架；GitHub Actions；just
- Python `re` 文档（含 greedy quantifiers）；regex101
- pyparsing（PEG，复杂解析）
- 开源项目中的 `pyproject.toml`、`.github/workflows/`、`DEVELOPMENT.md` 作为学习范例
