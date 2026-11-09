# Beyond the Code

原讲义：https://missing.csail.mit.edu/2026/beyond-code/

## 本讲目标

- 认识到优秀工程师不只写「能跑的代码」，还要写别人（含未来的自己）能理解、维护、继续构建的内容。
- 掌握**单向沟通**中「写 why 而非只写 what」：注释、README 漏斗、commit 的 Problem → Solution → Implications。
- 学会协作中的贡献礼仪：高质量 bug 报告、PR、审查；以及教育场景下如何提问。
- 理解 **AI etiquette**：有意义使用时披露、遵守团队规范、学习与考核场景中的边界。

## 知识点

### 核心原则：写 why，不只写 what

- 大量工程写作面向**缺乏你当前上下文**的人：后来的队友、接手维护者、六个月后的自己。
- **what** 往往自解释；**why** 是来之不易、易随时间丢失的知识——目标是捕获并传达 why。
- 尊重读者：开始认真写技术文字后，容易过度解释；应解释 why，信任读者自己弄清 how。

### 代码注释（Comments）

- 许多注释无用，但好注释解释**代码本身说不清的事**：为何如此做，而非如何工作（如何由代码展示）。
- 好注释可省数小时困惑；坏注释增加噪音，甚至误导。

几乎总是值得写的注释类型：

| 类型 | 要点 |
|------|------|
| TODOs | 标记未完成/未打磨处，留足上下文说明缺什么、为何推迟。「TODO: optimize」无用；「TODO: 此 O(n²) 对 n&lt;100 可接受，扩展需索引」才可行动。 |
| References | 算法论文、改编来源、规范文档；用 **permalink**；注明与参考的分歧。 |
| Correctness arguments | 非平凡正确性论证：代码是步骤，注释解释为何这些步骤正确。 |
| Hard-learned lessons | 花 30+ 分钟才调通的非显然写法——过去的自己没意识到需要，未来读者也不会。 |
| Rationale for constants | 魔法数字：为何 1492、为何 16 bits；随机选、测出来、还是正确性要求？「任意选定」也有信息量。 |
| Load-bearing choices | 正确性依赖看似无害的细节（如「必须用 BTreeSet，因为下面依赖迭代顺序」）。 |
| “Why not”s | 故意避开显然做法时写清原因，否则后人会「修好」并搞坏。 |

### README 漏斗

- README 常是与其他开发者的第一接触点。
- 好 README **立刻**回答四个问题（按此顺序）：
  1. What does this do?
  2. Why should I care?
  3. How do I use it?
  4. How do I install it?
- 结构像**漏斗**：顶部一句话 + 可选视觉 demo，几秒内决定是否相关；再逐步加深。
- **先展示用法，再写安装**——人们想先看到能得到什么，再承诺安装步骤。

### Commit messages

- 常被写成 “fixed blah” / “added foo”；但它们是代码库演化的**历史记录**。
- 别人（或你自己）`git blame` 时，好 commit message 应给出答案。
- Body 一般应回答：
  - 什么问题迫使这次改动？
  - 考虑过哪些替代方案？
  - 权衡与影响是什么？
  - 此方案有何可能令人惊讶之处？
- 细节随复杂度缩放：一行 typo 只需 subject；花数小时的竞态修复值得数段说明。

**Problem → Solution → Implications**（复杂改动）：

1. **Problem**：迫使改动的约束/局限
2. **Solution**：改了什么、关键设计决策
3. **Implications**：正负后果——记录「有意权衡」可防止后人以为你漏了问题

**用 LLM 写 commit 的注意点**：

- 只把 diff 丢给 LLM → 多半只有 what，生成描述性消息（与目标相反）。
- 若改动本身在同一次 LLM 会话中完成，会话已是丰富 why 来源，同会话写 commit 更好。
- 或明确要求聚焦 why，并让 LLM **向你追问缺失上下文**——你相当于 agent 的「工具」。

**提交粒度**：

- 逻辑拆分 commit（`git add -p` 是朋友）；每个 commit 应是可独立理解/审查的连贯改动。
- 不要把重构与新功能、不相关 bugfix 混在一起；利于审查与 `git bisect`。

### Collaboration：Contributing

- 用户 ≫ 贡献者 ≫ 维护者；维护者时间严重超订。贡献需**高信噪比**，值得维护者时间。

**好的 bug 报告**应包含：

- Environment：OS、版本、相关配置
- Expected vs actual
- Steps to reproduce：具体（「在 /settings 以 admin 登录后点 Submit」优于「点按钮」）
- What you’ve already tried：避免重复建议，表明已做调查

其他要点：

- **安全漏洞**：勿公开贴；先私下联系维护者并给合理修复时间；许多项目有 `SECURITY.md`。
- 先搜已有 issue，优先补充而非重复开单。
- **最小可复现示例（MRE）**是金矿；隔离问题有时自己就找到了修复。
- 维护者常是志愿者；几周后礼貌跟进可以，每日催不行。「me too」、只贴终端输出净收益常为负。
- 读并遵循 `CONTRIBUTING.md`；从小处开始（typo、文档）。
- 注意许可证：贡献落入同一 license；**copyleft**（如 GPL）要求衍生也开源，可能影响雇主——见 choosealicense.com。
- PR：隔离真正想被接受的改动；无关改动混在一起会被退回。多处改动若都服务于同一功能，大 PR 可接受，但需良好 commit 卫生以便逐 commit 审查。
- 解释 **why**；主动标出需特别关注的部分；按项目期望附上权衡与如何测试。
- 优先向上游贡献，而非先 fork；fork 留给超出原项目范围的改动，并致谢原项目。
- **AI 生成 PR**：容易产出看似合理的代码，但不免除你理解所贡献内容。提交自己都解释不清的 AI 代码，把负担推给维护者。可用 AI 辅助，但需自己打磨成值得的贡献。
- 接受 PR = 接受长期维护责任；维护者可因方向不符、复杂度、需求论证不足而拒绝——贡献者需论证为何值得维护负担。
- 收到反馈时：**代码不是你本人**；可澄清提问。

### Collaboration：Reviewing

- 审查并非只属资深者；新人视角有价值，问题常揭示应文档化或简化的假设。
- 审查是快速学习、在进生产前抓 bug、传播知识、协作提升质量的方式，不只是官僚手续。

好审查习惯：

- 评代码不评人：「这个函数令人困惑」而非「你写了令人困惑的代码」
- 可行动评论：「能否用 config dataclass 替换这些 globals」优于「别用 globals」
- 提问优于命令：「若 X 为 null 会怎样？」优于「处理 null」
- 解释 why：「用常量方便按环境调超时」优于仅「考虑用常量」
- 区分 **blocking** 与建议
- 肯定做得好的地方
- 知道何时停：别把所有 nit 耗尽贡献者耐心；大事优先，nit 可事后自己收拾

- AI 可作第一遍，**不能替代**理解产品需求与上下文的人类审查。

### Education：提问

- Julia Evans：*How to ask good questions*、*How to get useful answers to your questions*（讲义推荐）。
- 有价值的做法：
  - 先陈述自己的理解，再问「对吗？」
  - 问 yes/no：「Is X true?」减少跑题，常仍会引出有用展开
  - 具体：「LEFT JOIN 是否包含右表无匹配的行？」优于「SQL join 怎么工作？」
  - 不懂就打断问术语；不知就说 “I don’t know”，可加 “but I think …” / “but I can find out”
  - 不接受不完整答案，追问到真懂
  - 先做基本调研（同事间闲聊除外）
- 好问题惠及整个社区；上述建议**同样适用于与 LLM 沟通**。

### AI etiquette

- 规范仍在形成；战术细节见 Agentic Coding 讲，本讲谈「软」规范。
- **有意义贡献时披露**：诚实、设定期望、保证适当审查级别；说明用在哪些部分（整份 vibecode vs 只用 LLM 给前端加样式）。讲义承认 LLM 参与了部分讲义（校对、头脑风暴、代码/练习初稿）。
- 遵守团队/项目规范（合规、数据驻留等）；公开使用可避免代价高昂的失误。
- **学习时**：让 AI 包办大部分工作可能自我击败——学到的更像 prompting/审 AI，而非任务本身；学习时「快速拿解」可能是反目标。
- **面试/考核**：常在评你而非 LLM；少数公司允许用 AI 但观察你如何使用；不确定就问。若明确禁止外部工具/LLM，**不要用**，偷偷用会反噬。

## 代码/命令示例

### 无用 vs 可行动的 TODO（讲义对比）

```
TODO: optimize
```

```
TODO: this O(n²) loop is fine for n<100, but will need indexing if we scale
```

**作用：** 说明 TODO 需留下「缺什么、为何可暂缓、何时必须改」的上下文。

### Load-bearing 选择类注释意图（讲义举例）

```
# must be a BTreeSet because iteration order matters below
```

**作用：** 标出看似无害却关乎正确性的实现细节，防止后人「好心」改坏。

### 用 git 查看弱 commit 并重写练习相关命令

```bash
git log
git show <commit>
```

**作用：** 练习对比好/弱 commit message；对弱消息按 Problem → Solution → Implications 重写（练习 2）。

### 部分暂存以拆分逻辑 commit

```bash
git add -p
```

**作用：** 把不相关改动拆成语义连贯、可独立审查的 commit。

## 练习要点

1. 浏览知名项目源码（如 Redis、curl），找讲义中的注释类型（有用 TODO、外部文档引用、why not、hard-learned lesson）；思考没有该注释会丢什么。
2. 看感兴趣开源项目的 `git log`：找一条解释 why 的好消息与一条只写 what 的弱消息；对弱消息看 `git show` 并按 Problem → Solution → Implications 重写——体会事后拼上下文有多费劲。
3. 对比三个 1000+ stars 项目的 README：是否同样有用？哪些对你主要是噪音。
4. 找一个 open issue（可看 good first issue / help wanted），对照讲义标准：是否尊重维护者时间、信息是否够 debug，还是预期要多轮追问。
5. 对真实遇到的 bug（或 issue tracker）练习最小可复现：剥到仍能复现的最小例子；写清去掉了什么及原因。
6. 找有实质审查评论（非仅 LGTM）的已合并 PR；评估评论生产力；若你是作者，收到那些评论的体验如何。
7. 在 Stack Overflow 找高票问题与被关/狂踩问题，对照讲义提问建议，看是否可预测。

## 延伸阅读

- 本课：https://missing.csail.mit.edu/2026/beyond-code/
- Julia Evans：How to ask good questions；How to get useful answers to your questions
- choosealicense.com（许可证选择）
- Agentic Coding（AI 战术与工具）：https://missing.csail.mit.edu/2026/agentic-coding/
- Version Control（commit / bisect 相关）：https://missing.csail.mit.edu/2026/version-control/
