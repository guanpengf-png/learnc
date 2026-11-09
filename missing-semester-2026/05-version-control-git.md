# Version Control and Git（版本控制与 Git）

原讲义：https://missing.csail.mit.edu/2026/version-control/

## 本讲目标

理解版本控制系统（VCS）为何有用，以及为何要从 **Git 数据模型自底向上**学习，而不是把命令当「魔法咒语」死记。掌握 blob / tree / commit DAG、内容寻址、引用与 HEAD、暂存区，以及常用 CLI（基础、分支合并、远程、撤销、进阶），并能完成讲义中的练习（blame、stash、alias、合并冲突等）。

---

## 知识点

### 一、版本控制的用途

VCS 跟踪源码（或其它文件/文件夹集合）的变更：维护历史，并便于协作。逻辑上，它们把某顶层目录下文件/文件夹的状态记成一系列**快照**，并保存元数据（谁创建、提交说明等）。

即使单人开发，也能：查看旧快照、记录「为何改」、并行分支开发等。协作时：看别人改了什么、解决并发冲突。现代 VCS 还能回答：谁写的这个模块？某行何时由谁因何修改？最近 1000 次修订里某单元测试何时/为何挂了？

Git 是事实标准。其界面是 leaky abstraction，自上而下只背命令容易混乱（如 XKCD 所示）。底层设计优美：界面需背，设计可理解——故本讲自底向上：先数据模型，再 CLI。

### 二、Git 数据模型

#### Snapshots：blob 与 tree

历史 = 某顶层目录内文件/文件夹的一系列快照。

- **blob**：文件，一堆字节
- **tree**：目录，名字 → blob 或 tree（可嵌套）
- **快照**：被跟踪的顶层 tree

示例结构：

```
<root> (tree)
|
+- foo (tree)
|  |
|  + bar.txt (blob, contents = "hello world")
|
+- baz.txt (blob, contents = "git is wonderful")
```

#### 历史：Commit 的 DAG

Git **不用**简单线性列表。历史是快照的**有向无环图（DAG）**：每个快照引用一组 **parents**（前序快照）。多个 parent 是因为合并并行分支会产生多父提交。

这些快照叫 **commit**。ASCII 示意：箭头指向 parent（「先于」关系）。第三提交后可分出两支，之后合并得到带合并提交的新历史。

**Commit 不可变**：修正历史不是改旧提交，而是创建新提交，并更新引用指向新提交。

#### 伪代码模型

```
// a file is a bunch of bytes
type blob = array<byte>

// a directory contains named files and directories
type tree = map<string, tree | blob>

// a commit has parents, metadata, and the top-level tree
type commit = struct {
    parents: array<commit>
    author: string
    message: string
    snapshot: tree
}
```

#### Objects 与内容寻址（SHA-1）

`object = blob | tree | commit`。对象库用 **SHA-1** 内容寻址：

```
objects = map<string, object>

def store(object):
    id = sha1(object)
    objects[id] = object

def load(id):
    return objects[id]
```

对象相互引用时存的是**哈希指针**，不内嵌对方内容。例如某 tree（`git cat-file -p …`）可显示：

```
100644 blob 4448adbf7ecd394f42ae135bbeed9676e894af85    baz.txt
040000 tree c68d233a33c5c06e0340e4c224f0afca87c8ce87    foo
```

再对 blob 哈希 `cat-file -p` 得到内容 `git is wonderful`。

#### References 与 HEAD

40 位十六进制不便记忆 → **references**：指向 commit 的可变指针（对象不可变，引用可变）。如 `master` 通常指向主线最新提交。

特殊引用 **HEAD**：当前「在历史中的位置」，新建 commit 时据此设 `parents`。

#### Repository

仓库 ≈ `objects` + `references`。磁盘上 Git 存的就是这些；所有 `git` 命令都是在 DAG 上增对象、增/改引用。打命令时想「对图做了什么」；想改图（如丢弃未提交改动并把 `master` 指到某 commit）则找对应命令（如 `git checkout master; git reset --hard 5d83f9e`）。

### 三、Staging Area（暂存区）

与数据模型正交，但是创建 commit 的接口的一部分。

并非「一键把整个工作区打成快照」：希望干净快照——例如两功能想分两次提交；或修好了 bug 但到处有 debug print，只想提交修复、丢弃打印。暂存区让你**指定下次快照包含哪些修改**。

### 四、Git 命令行接口

讲义不逐条详解，推荐 **Pro Git** 或看视频。命令按数据模型理解即可。

#### Basics

- `git help <command>`：帮助
- `git init`：新建仓库，数据在 `.git`
- `git status`：当前状态
- `git add <filename>`：加入暂存区
- `git commit`：创建提交（写好 commit message！讲义链到两篇「为何写好提交说明」）
- `git log`：扁平历史
- `git log --all --graph --decorate`：DAG 可视化
- `git diff <filename>`：相对暂存区的改动
- `git diff <revision> <filename>`：快照间文件差异
- `git checkout <revision>`：更新 HEAD（若检出分支则当前分支随之）

#### Branching and merging

- `git branch` / `git branch <name>`：列出 / 创建分支
- `git switch <name>`：切换分支
- `git checkout -b <name>`：创建并切换（= `git branch` + `git switch`）
- `git merge <branch>`：合并进当前分支
- `git mergetool`：图形/终端工具解决冲突
- `git rebase`：把一组补丁变基到新基底

#### Remotes

- `git remote` / `git remote add <name> <url>`
- `git push <remote> <local branch>:<remote branch>`：发送对象并更新远程引用
- `git branch --set-upstream-to=<remote>/<branch>`：本地与远程跟踪关系
- `git fetch`：取回对象/引用
- `git pull`：≈ `fetch` + `merge`
- `git clone`：从远程下载仓库

#### Undo

- `git commit --amend`：改最近提交内容/说明
- `git reset <file>`：取消暂存
- `git restore`：丢弃工作区改动

#### Advanced Git

- `git config`：高度可定制
- `git clone --depth=1`：浅克隆
- `git add -p`：交互式暂存
- `git rebase -i`：交互式 rebase
- `git blame`：每行最后谁改
- `git stash`：暂存工作区修改并清干净
- `git bisect`：历史二分查回归
- `git revert`：新建提交以撤销某次提交效果
- `git worktree`：同时检出多个分支
- `.gitignore`：忽略故意不跟踪的文件

### 五、杂项

- **GUI**：很多，讲义作者偏好 CLI。
- **Shell 集成**：提示符显示 Git 状态（Oh My Zsh 等常自带）。
- **编辑器集成**：如 Vim 的 fugitive.vim。
- **工作流**：讲了模型与基础命令，大项目实践有多种流派。
- **GitHub ≠ Git**：协作常通过 pull request；另有 GitLab、Bitbucket 等托管。

---

## 代码/命令示例

### 数据模型伪代码与内容寻址

```
type object = blob | tree | commit

objects = map<string, object>

def store(object):
    id = sha1(object)
    objects[id] = object

def load(id):
    return objects[id]
```

**作用：** 说明 blob/tree/commit 统一为 object，并用 SHA-1 内容寻址存取；引用其他对象时存哈希而非内嵌。

### 引用的读写（伪代码）

```
references = map<string, string>

def update_reference(name, id):
    references[name] = id

def read_reference(name):
    return references[name]

def load_reference(name_or_id):
    if name_or_id in references:
        return load(references[name_or_id])
    else:
        return load(name_or_id)
```

**作用：** 用可变的人类可读名字指向不可变 commit；`HEAD` 是特殊引用，表示当前位置。

### 查看对象内容

```bash
git cat-file -p 698281bc680d1995c5f4caaf3359721a5a58d48d
# 示例输出：
# 100644 blob 4448adbf7ecd394f42ae135bbeed9676e894af85    baz.txt
# 040000 tree c68d233a33c5c06e0340e4c224f0afca87c8ce87    foo

git cat-file -p 4448adbf7ecd394f42ae135bbeed9676e894af85
# git is wonderful
```

**作用：** 按哈希打印 tree/blob 内容，验证「tree 存指针、blob 存文件字节」的模型。

### 可视化 DAG 与常用基础流

```bash
git log --all --graph --decorate
git add <filename>
git commit
git status
```

**作用：** 以图方式看分支/合并结构；`add` 写入暂存区，`commit` 从暂存区创建不可变快照并更新引用。

### 分支、合并与变基

```bash
git branch feature
git switch feature
# 或：git checkout -b feature
git merge feature
git mergetool
git rebase
```

**作用：** 创建/切换并行开发线；`merge` 产生可多父的合并提交；`mergetool` 辅助解决冲突；`rebase` 把补丁接到新基底上改写历史形态。

### 远程协作

```bash
git remote add origin <url>
git push origin main:main
git fetch
git pull
git clone <url>
```

**作用：** 在本地 objects/references 与远程之间同步；`pull` ≈ fetch 后再 merge。

### 撤销与进阶常用

```bash
git commit --amend
git reset <file>
git restore <file>
git blame <file>
git stash
git stash pop
git bisect
git revert <commit>
git clone --depth=1 <url>
git add -p
git rebase -i
```

**作用：** amend/reset/restore 处理「刚提交错了 / 暂存错了 / 工作区改坏了」；blame 追责到行；stash 临时腾出干净工作区；bisect 二分回归；revert 用新提交反向某次改动而不改写已共享历史。

### 别名与全局 ignore（练习相关）

```bash
# 使 git graph ≡ git log --all --graph --decorate --oneline
git config --global alias.graph "log --all --graph --decorate --oneline"

git config --global core.excludesfile ~/.gitignore_global
# 然后编辑 ~/.gitignore_global，例如加入 .DS_Store
```

**作用：** 缩短常用长命令；全局忽略 OS/编辑器临时文件，避免误加入仓库。

### 合并冲突标记（recipe 练习）

冲突文件中会出现：

```
<<<<<<<
=======
>>>>>>>
```

**作用：** 分别标出「当前分支一侧」「另一侧」与边界；编辑保留所需内容并删除标记后，`git add` + `git commit`（或 `git merge --continue`）完成合并。

---

## 练习要点

1. **入门**：无经验则读 Pro Git 前几章或玩 [Learn Git Branching](https://learngitbranching.js.org/)，把每个命令对应到对 DAG/引用的操作。
2. **克隆课程网站仓库**：
   - 用图可视化历史；
   - 谁最后改了 `README.md`？（`git log` 带路径参数）；
   - `_config.yml` 中 `collections:` 那行最后一次修改的 commit message？（`git blame` + `git show`）。
3. **从历史删除大文件/敏感信息**：先提交再从历史抹掉（不只删最新提交）；讲义指向相关指南（BFG/filter 一类流程）。
4. **stash**：改已有文件后 `git stash`；看 `git log --all --oneline`；`git stash pop` 恢复。思考何时有用（如临时切分支处理紧急 bug）。
5. **`~/.gitconfig` alias**：`git graph` → `git log --all --graph --decorate --oneline`。
6. **全局 gitignore**：`core.excludesfile` → `~/.gitignore_global`，忽略 `.DS_Store` 等。
7. **Fork + PR**：对课程网站提有用改进的 PR（勿刷垃圾 PR）；找不到可跳过。
8. **模拟合并冲突（recipe）**：
   1. `git init`，写 `recipe.txt` 并提交；
   2. `git branch salty` / `git branch sweet`；
   3. 在 salty 改同一行并提交（如 sugar→salt）；
   4. 在 sweet 改同一行不同内容并提交；
   5. 在 master `merge salty` 再 `merge sweet`，观察冲突与 `<<<<<<<` 等标记；
   6. 手改或 `git mergetool` 解决后完成合并；
   7. `git log --graph --oneline` 看合并历史。

---

## 延伸阅读

- [Pro Git](https://git-scm.com/book/en/v2)（强烈推荐；第 1–5 章够日常，后文进阶）
- [Oh Shit, Git!?!](https://ohshitgit.com/)：常见失误恢复
- [Git for Computer Scientists](https://eagain.net/articles/git-for-computer-scientists/)：数据模型图解
- [Git from the Bottom Up](https://jwiegley.github.io/git-from-the-bottom-up/)：实现细节
- How to explain git in simple words（讲义所列）
- [Learn Git Branching](https://learngitbranching.js.org/)：浏览器小游戏
- 原讲义：https://missing.csail.mit.edu/2026/version-control/
