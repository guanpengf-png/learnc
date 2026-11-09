# The Missing Semester of Your CS Education（2026）中文学习笔记

课程主页：https://missing.csail.mit.edu/  
2026 讲座列表：https://missing.csail.mit.edu/2026/

## 课程概览

CS 课程常讲授操作系统、机器学习等高级主题，却很少系统教**工具熟练度**——多半留给学生自学。本课补上这一「缺失的学期」：掌握命令行、强大的编辑器、版本控制等高生产力工具，以及相关工作流。

学生在求学期间会花数百小时（职业生涯数千小时）使用这些工具；熟练使用不仅减少摩擦，还能解决原先看似不可能的问题。

2026 年版特别强调：软件工程正因 **AI 赋能/增强** 的工具与工作流而快速变化。在恰当使用并清醒认识局限时，这些工具能显著帮助从业者。AI 是横切能力，**没有单独的 AI 专讲**；最新适用的 AI 工具与技巧已折入各讲。

- **授课**：Anish、Jon、Jose 联合授课
- **提问**：missing-semester@mit.edu
- **讨论**：OSSU Discord（`#missing-semester-forum` 类 Piazza；`#missing-semester` 闲聊）
- **录像**：讲座录像在 YouTube 可看
- **延伸阅读动机**：课程站点 “Read about the motivation behind this class”

本目录笔记为复习向整理：中文叙述、英文代码/命令；结构对齐讲义，不臆造未出现的内容。

## 2026 教学大纲（9 讲）

| # | 日期 | 主题 | 原讲义 | 本目录笔记 |
|---|------|------|--------|------------|
| 1 | 1/12/26 | Course Overview + Introduction to the Shell | [course-shell](https://missing.csail.mit.edu/2026/course-shell/) | [01-shell.md](./01-shell.md) |
| 2 | 1/13/26 | Command-line Environment | [command-line-environment](https://missing.csail.mit.edu/2026/command-line-environment/) | [02-command-line-environment.md](./02-command-line-environment.md) |
| 3 | 1/14/26 | Development Environment and Tools | [development-environment](https://missing.csail.mit.edu/2026/development-environment/) | [03-development-environment.md](./03-development-environment.md) |
| 4 | 1/15/26 | Debugging and Profiling | [debugging-profiling](https://missing.csail.mit.edu/2026/debugging-profiling/) | [04-debugging-profiling.md](./04-debugging-profiling.md) |
| 5 | 1/16/26 | Version Control and Git | [version-control](https://missing.csail.mit.edu/2026/version-control/) | [05-version-control-git.md](./05-version-control-git.md) |
| 6 | 1/20/26 | Packaging and Shipping Code | [shipping-code](https://missing.csail.mit.edu/2026/shipping-code/) | [06-shipping-code.md](./06-shipping-code.md) |
| 7 | 1/21/26 | Agentic Coding | [agentic-coding](https://missing.csail.mit.edu/2026/agentic-coding/) | [07-agentic-coding.md](./07-agentic-coding.md) |
| 8 | 1/22/26 | Beyond the Code | [beyond-code](https://missing.csail.mit.edu/2026/beyond-code/) | [08-beyond-code.md](./08-beyond-code.md) |
| 9 | 1/23/26 | Code Quality | [code-quality](https://missing.csail.mit.edu/2026/code-quality/) | [09-code-quality.md](./09-code-quality.md) |

## 各讲一句话

1. **Shell**：文本接口、路径/`$PATH`、管道重定向与基础 Shell 编程。
2. **Command-line Environment**：CLI 约定、参数与标志、流、dotfiles。
3. **Development Environment**：IDE vs 终端工作流、编辑器/语言服务、AI 辅助开发入门。
4. **Debugging and Profiling**：打印/日志、调试器、性能分析。
5. **Version Control**：Git 模型与日常命令、协作工作流。
6. **Shipping Code**：制品与打包、环境假设、交付与部署形态。
7. **Agentic Coding**：编码智能体、tool-calling、上下文管理与用例/陷阱。
8. **Beyond the Code**：注释/README/commit 的 why、协作与提问、AI 礼仪。
9. **Code Quality**：格式化、lint、测试、pre-commit、CI/CD、命令运行器与正则。

## 往年专题（2026 未覆盖，可按需补看）

课程主题逐年调整。2026 未讲、往年有的专题包括（详见课程主页）：

- 2020：Data Wrangling；Security and Cryptography；Potpourri；Q&A
- 2019：Backups；Automation；Machine Introspection；OS Customization；Web and Browsers；Security and Privacy

## 笔记体例

每讲 Markdown 通常包含：

- 标题 + 原讲义 URL
- `## 本讲目标`
- `## 知识点`
- `## 代码/命令示例`（`###` 小节 + 代码 + **作用：**）
- `## 练习要点`
- `## 延伸阅读`

## 致谢（课程原致谢）

感谢 Elaine Mello 与 MIT Open Learning 支持讲座录像；感谢 Luis Turino / SIPB 将本课纳入 SIPB IAP 2026。
