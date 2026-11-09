# Agentic Coding

原讲义：https://missing.csail.mit.edu/2026/agentic-coding/

## 本讲目标

- 理解 **coding agents**（编码智能体）：会对话、能调用工具（读写文件、搜索网页、执行 shell）的 AI 模型，可运行在 IDE 或独立 CLI/GUI 中。
- 建立高层心智模型：LLM 是「给定 prompt 的补全字符串概率分布」；固定 **context window**；tool-calling 由 harness 解释工具请求并回填结果。
- 掌握常见用例：实现功能、修错、重构、代码审查、理解代码库、当自然语言 shell、vibe coding。
- 了解进阶能力：可复用 prompt/skills、并行 agents + git worktrees、MCP、上下文管理（clear / rewind / compact / `llms.txt` / `AGENTS.md` / skills / subagents）。
- 认清局限与风险：隐私、错误与安全 bug、过度依赖；按练习亲手对比手工/补全/chat/agent。

## 知识点

### Coding agents 是什么

- Coding agents = 对话式 AI + 工具访问（读/写文件、网页搜索、调用 shell 等）。
- 可住在 IDE 内，或独立命令行 / GUI 工具中。
- 高度自治且强大；本讲建立在「Development Environment and Tools」讲中的 AI 开发材料之上。
- 默认情况下，agent harness 会在可能有害的工具调用前**提示用户确认**。
- 支持多轮交互；可中途打断纠正方向。心智模型：**你像管理实习生**——对方做琐碎实现，但需要指导，偶尔做错需纠正。

### Demo：把脚本改成 argparse CLI

- 起点脚本：下载 URL 内容 → 用正则抽取 Markdown 链接 → 打印结果。
- Prompt 示例：改成带 `argparse` 的正式命令行程序；加类型注解；确保通过类型检查。
- Agent 典型行为：读文件理解 → 编辑 → 调用类型检查器；失败则迭代。
- 若 agent 用错命令（例如本机有 `mypy`，却调用 `python -m mypy`），可用文字反馈纠正。
- 跟进练习：让 agent 运行脚本；观察输出；再要求只保留绝对 URL 等变更。

### LLM 与 agents 如何工作（高层）

- LLM 可看作：在给定 prompt（输入）下，对补全字符串（输出）建模概率分布；推理时从该分布采样。
- **Context window**：输入 + 输出的最大长度，固定有限。
- 多轮对话：用 turn markers，每次把**整段对话历史**作为新 prompt，每用户一轮触发一次推理。
- **Tool-calling loop**：harness 把某些 LLM 输出解释为「调用工具」；工具结果再写回 prompt；每次 tool 调用/响应都会再跑一轮 LLM。核心概念可用约 200 行代码实现。

### Privacy（隐私）

- 多数 AI 编程工具默认配置会把大量数据发到云端。
- 可能是：harness 本地、推理在云；也可能更多软件在云端运行——服务商可能拿到整个仓库副本及全部交互。
- 开源 AI 编程工具与开源 LLM 已可用（通常不及闭源顶尖模型）；对多数用户，本地跑前沿开源 LLM 仍因硬件不可行。

### 用例

| 用例 | 要点 |
|------|------|
| Implementing new features | 规格描述是门艺术：够清晰让 agent 朝正确方向走，又不至于自己做完所有工作。**TDD** 很有效：先写/审测试，再让 agent 实现。模型在进化，需不断更新直觉。讲义 sidenotes 用 Claude Code 实现。 |
| Fixing errors | 可把编译器/linter/类型检查/测试错误交给 agent（如「fix the issues with mypy」）。能让模型**自己跑失败检查并迭代**时最有效；否则可手动反馈。 |
| Refactoring | 从重命名到拆模块；简单重构也可用 code intelligence。讲义曾用 Claude Code 把 agentic coding 拆成独立一讲。 |
| Code review | 「审查尚未提交的最新改动」；若支持 web fetch 或已装 `gh`，可「Review the pull request {link}」。 |
| Code understanding | 对陌生代码库提问，适合 onboarding。 |
| As a shell | 用自然语言驱动具体工具，如 find 找 30 天前文件、mogrify 缩放 jpg。 |
| Vibe coding | 强大到可不手写一行代码实现部分应用；讲义有讲师真实 vibe-code 项目示例。 |

### Advanced agents

#### Reusable prompts / skills

- 可写详细、可复用的 prompt（如固定风格的 code review）。
- 工具演进快：部分工具中「可复用 prompt」已弃用，被 **skills** 吸收（如 Codex、Claude Code）。

#### Parallel agents + worktrees

- Agent 可能跑数十分钟；可并行多个副本：同一任务多跑几次取最优（LLM 有随机性），或不同任务并行。
- 用 **git worktrees** 隔离改动，避免互相干扰（见 Version Control 讲）。

#### MCP（Model Context Protocol）

- 开放协议，把 coding agents 接到外部工具（如 Notion MCP：读/写 Notion，实现「读规格 → 写计划页 → 实现原型」）。
- 发现 MCP：可用 Pulse、Glama 等目录。

#### Context management

- 目标：agent 有所需信息，但避免无关上下文撑爆或劣化性能（即使未溢出，上下文变大也常降质）。
- Harness 会自动供给并部分管理上下文，但大量控制仍在用户。

| 手段 | 说明 |
|------|------|
| Clearing | 清空上下文 / 新开对话；**无关查询应新开**。 |
| Rewinding | 部分 agent 支持撤销对话步骤；比「再发一条纠偏」更有效管理上下文。 |
| Compaction | 历史过长时，自动用 LLM 摘要前缀并替换历史；有的允许用户主动触发。 |
| `llms.txt` | 提议的标准路径：给 LLM 推理时读的密集文档（产品/库/API 常提供）。比抓 HTML 更省 token；适合知识截止日期之后的依赖文档。 |
| `AGENTS.md` | 多数 agent 支持（Claude Code 看 `CLAUDE.md`）。启动时**整文件**预填上下文：跨会话建议（改完必跑类型检查、如何跑测、第三方文档链接等）。有的可用 `/init` 自动生成。 |
| Skills | `AGENTS.md` 总是全量加载；skills 多一层间接：先给技能列表+描述，agent **按需打开**再加载，减轻上下文膨胀。 |
| Subagents | 顶层 agent 可调用任务型子 agent；顶层不被子任务细节撑爆，子 agent 只拿任务所需上下文。例：网页研究子 agent——搜索、读页、分析后只把答案回顶层。 |

- 写 skills/subagents 的 prompt 时可先让 LLM 起草；Claude Code 可用 `/agents` 从短 prompt 生成子 agent。
- 示例子 agent prompt：用 `mypy` 与 `ruff` 对相对上次 commit 改动的文件做类型检查、lint、format check；再用「use the code checker subagent」显式调用，或让顶层在改 Python 后自动调用。

### What to watch out for（坑）

- AI 会错：底层是概率 next-token 模型，并非人类式「智能」。
- **审查正确性与安全 bug**；有时验证比自己写更难——关键代码可考虑手写。
- 可能钻牛角尖、gaslight；警惕调试螺旋。
- 勿当拐杖；警惕浅层理解与过度依赖。仍有大量编程任务 AI 做不了；计算思维仍有价值。
- 流行工具：IDE/扩展内 agents；Anthropic Claude Code、OpenAI Codex；开源如 opencode。

## 代码/命令示例

### 起点脚本（下载 + 抽取 Markdown 链接）

```python
from urllib.request import urlopen

def download_contents(url: str) -> str:
    with urlopen(url) as response:
        return response.read().decode('utf-8')

def extract(content: str) -> list[str]:
    import re
    pattern = r'\[.*?\]\((.*?)\)'
    return re.findall(pattern, content)

print(extract(download_contents("https://raw.githubusercontent.com/missing-semester/missing-semester/refs/heads/master/_2026/development-environment.md")))
```

**作用：** Demo 起点；后续交给 agent 改成正式 CLI。

### 交给 agent 的任务 prompt

```
Turn this into a proper command-line program, with argparse for argument parsing. Add type annotations, and make sure the program passes type checking.
```

**作用：** 演示「规格足够清晰」的 agent 任务：改 CLI、加类型、跑类型检查并迭代。

### 自然语言 shell 类 prompt（讲义举例）

```
use the find command to find all files older than 30 days
```

```
use mogrify to resize all the jpgs to 50% of their original size
```

**作用：** 把 agent 当 shell——指定工具 + 自然语言目标。

### 生成「代码检查」子 agent 的 prompt

```
A Python code checking agent that uses `mypy` and `ruff` to type-check, lint, and format *check* any files that have been modified from the last git commit.
```

**作用：** 创建任务型 subagent；顶层可显式或自动调用，以隔离检查相关上下文。

### 修错类 prompt（讲义举例）

```
fix the issues with mypy
```

**作用：** 把 agent 放进「跑检查 → 修 → 再跑」反馈环。

## 练习要点

1. **四种方式对比**：同一编程任务分别用手写、AI 补全、inline chat、agents 各做一遍（小功能 / good first issue / Advent of Code / LeetCode）。
2. **陌生代码库导航**：最好在真实想 debug/加功能的项目上；否则可用 agent 理解 opencode 的安全相关功能。
3. **Vibe coding**：从不手写一行代码，从零 vibe 出一个小应用。
4. **配置 `AGENTS.md`（或 `CLAUDE.md`）、skill、subagent** 并测试；思考三者分别何时用；若不支持可换工具或跳过。
5. **对照 Code Quality 讲的 Markdown 列表正则练习**：agent 是否直接改文件？直接改的局限？如何 prompt 让它用第一讲提到的命令行工具而非直接编辑？（提示：让它用第一讲的 CLI 工具。）
6. **隔离环境 + yolo 模式**：`--dangerously-skip-permissions` 等不安全直接用，但可在 VM/容器等隔离环境中开启自治；参考 Claude Code devcontainers、Docker Sandboxes 等文档。

## 延伸阅读

- 本课：https://missing.csail.mit.edu/2026/agentic-coding/
- Development Environment and Tools（AI 开发基础）：https://missing.csail.mit.edu/2026/development-environment/
- Version Control（git worktrees）：https://missing.csail.mit.edu/2026/version-control/
- Code Quality（Markdown bullet 正则练习对照）：https://missing.csail.mit.edu/2026/code-quality/
- MCP 发现目录（讲义提及）：Pulse、Glama
- 示例文档：`llms.txt`（如 cursor.com/llms.txt、ai.pydantic.dev/llms.txt、apify.com/llms.txt）；`AGENTS.md` 真实示例见讲义链接
- 工具：Claude Code、OpenAI Codex、opencode
