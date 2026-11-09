# Packaging and Shipping Code（打包与交付代码）

原讲义：https://missing.csail.mit.edu/2026/shipping-code/

## 本讲目标

让代码在自己机器上按预期工作已经很难；让**同一份代码在别人机器上跑起来**往往更难。本讲定义 **shipping = 交付物（artifact）+ 对环境的假设**，并以 Python 生态为例讲清：依赖与环境、制品与打包、发布与版本、可复现性、VM/容器、配置、服务与编排、发布分发。工具因语言而异，概念大体相通。

本讲大纲：Dependencies & Environments → Artifacts & Packaging → Releases & Versioning → Reproducibility → VMs & Containers → Configuration → Services & Orchestration → Publishing。

---

## 知识点

### 一、Dependencies & Environments（依赖与环境）

现代软件层层抽象，程序依赖库/服务。例如 `import requests`——`requests` 不随 Python 运行时自带，未安装会 `ModuleNotFoundError`。

`pip install requests` 大致步骤：

1. 在 PyPI 搜索；
2. 选适合当前平台的制品；
3. **解析依赖**（含传递依赖的兼容版本）；
4. 下载、解压并安装到文件系统正确位置。

不同语言工具不同：Rust 的 `cargo` 统一构建/测试/依赖/发布；Python 在规范层统一（PEP），工具可竞争（`pip` vs `uv`，`setuptools`/`hatch`/`poetry`）；LaTeX 发行版常预装大量包。

**依赖冲突 / dependency hell**：多包对同一库要求互不兼容的版本范围。缓解：为每个程序隔离环境。Python：`python -m venv` 创建虚拟环境——可视为带独立已装包集合的「独立运行时」。**不要改 OS 自带的语言运行时**（系统可能依赖它）。

Python：PEP 517（构建接口）、PEP 621（`pyproject.toml` 元数据）催生更快的 `uv`。推荐尽量用 `uv pip` 代替 `pip`。环境还可固定**不同 Python 版本**（`uv venv --python 3.12` 等）。许多语言项目会自动隔离依赖；也有多版本语言管理器。

### 二、Artifacts & Packaging（制品与打包）

**源码** vs **artifact（制品）**：制品是可安装/可部署的打包输出——从单个可运行文件到整台 VM。仅本地有 `greet.py` 时，换目录可能 `ModuleNotFoundError`（Python 只在特定路径找模块）。**打包**把代码装到已知位置。

Python 制品常称 **wheel**（含代码、元数据、安装说明）。项目清单用 **`pyproject.toml`**（现代推荐；`requirements.txt`/`setup.py` 仍可用但应优先 toml）。

示例：库 + CLI（`typer`），`[project.scripts]` 暴露 `greet` 入口；`uv build` 产出 `.whl` 与源码包 `.tar.gz`。他人 `uv pip install` 该 wheel 后可直接跑 `greet`。

局限：依赖平台库（如 CUDA）时需按平台/架构分别构建 wheel。

**从源码安装** vs **预编译二进制**：前者需编译器、耗时长；后者快但须匹配平台。如 ripgrep 为多 OS/架构提供预编译包。

### 三、Releases & Versioning（发布与版本）

开发连续，**发布离散**。dev 验证后再进 prod。发布含测试、依赖、版本、配置、部署、发布等。

库演进用版本号标识；**changelog** 说明变更。传递依赖使「跟踪每个依赖每个变更」不现实 → 版本约定。

**SemVer（语义化版本）**：`MAJOR.MINOR.PATCH`

- PATCH：修 bug，应向后兼容
- MINOR：兼容地加功能
- MAJOR：破坏性变更

这是简化；完整规范还解释 0.x 破坏性、`1.0.0-rc.1` 等。Python 依赖约束遵循 **PEP 440**。

**CalVer（日历版本）**：如 Ubuntu `24.04`——易看出新旧，不表达兼容性。SemVer 也非万无一失，维护者可能在 minor/patch 中无意引入破坏。

可用 `uv tree` 可视化依赖树。

### 四、Reproducibility（可复现性）

代码之下是运行时、第三方库、OS、硬件；任一层差异都可能改行为。**Pinning**：钉死精确版本（`==`）而非范围。

包管理器求解所有约束后，把具体版本列表写入 **lockfile**（如 `uv.lock`），保证可复现。

**库 vs 应用**：

- **库**：被别人导入，过严钉死易与用户其他依赖冲突 → 宜用版本**范围**；
- **应用/服务**：最终消费者 → 宜**钉死**以求人人一致。

极致可复现：**Nix**、**Bazel** 等 hermetic builds（编译器、系统库、构建环境皆钉死并可内容寻址）。也可用 NixOS 用版本控制管理整机配置。

新版本可能破坏，旧版本有安全洞 → CI 测新版本 + Dependabot 类自动化；升级仍出问题则要有 **rollback**。

### 五、VMs & Containers（虚拟机与容器）

依赖超出包管理器时（GPU 驱动、特定 OpenSSL、系统库等），传统方案是 **VM**（整机隔离、自带 OS）。更现代的是 **容器**：打包应用+依赖+文件系统，**共享宿主机内核**，更轻、启动更快。

**Docker** 标准化构建/分发/运行；底层常用 **containerd**（Kubernetes 等也用）。

- **Image**：打包好的模板制品；
- **Container**：该 image 的运行实例（可多开）。

Dockerfile 每条指令一层，可缓存。差 Dockerfile 问题：用过大基础镜像、拆太多 `RUN`、未钉版本、未清缓存、以 root 跑、层里嵌入密钥。改进示例：`python:3.12-slim`、合并 apt、`uv sync --locked`、从别的 image **COPY 预编译 `uv`（builder pattern）**。

限制：镜像常平台相关（amd64 vs arm64 需模拟则慢）；macOS/Windows 上 Docker 实际跑 Linux VM；容器隔离弱于 VM（共享内核，多租户有安全顾虑）。也可用 nix flakes 管理「系统级」依赖。

### 六、Configuration（配置）

软件应可配置；配置宜在**运行时**提供（标志、环境变量、配置文件/dotfiles），**勿硬编码**。同一代码库应仅靠配置差异部署到 dev/staging/prod。

密钥（API key 等）须小心，**不得进版本控制**。

### 七、Services & Orchestration（服务与编排）

典型 Web 应用：数据库、缓存、消息队列等。常拆成可独立开发/部署/扩缩的服务（微服务），网络通信（常 HTTP）。

与其把 Redis 编进同一容器（依赖难调和），不如各容器部署。**Docker Compose** 用一份 YAML 声明多服务、网络、卷；`docker compose up` 一并启动，服务名作 DNS 主机名。

生产可用 **systemd** 管 compose（开机自启、失败重启）。更大规模：**Kubernetes**——学习曲线陡，小项目常过重。

服务间常经标准 API；例如对 Anthropic/OpenAI 发 HTTP JSON 请求。

### 八、Publishing（发布分发）

最简：上传可下载制品（如 Ubuntu `.deb` 目录）。GitHub 成事实平台；**GitHub Releases** 可对 tag 挂预编译包。

包管理器可从 Git 或 Release URL 安装。Go 偏去中心（模块路径即源码地址）；多数（pip/cargo/brew）有中心索引。Wheel 名含平台标签：`py3-none-any` 通用；`cp312-...-macosx_..._arm64` 等为平台专用。

发布到 **PyPI**（可用 **TestPyPI** 练手）；`uv publish`。信任：校验和；部分生态有签名。各语言注册表：crates.io、npm、RubyGems、Docker Hub；组织可自建私有源。Web 服务还需域名、DNS、反向代理（nginx）等；静态站可用 GitHub Pages。

---

## 代码/命令示例

### 缺少依赖时的错误与 pip 安装

```bash
$ python fetch.py
Traceback (most recent call last):
  File "fetch.py", line 1, in <module>
    import requests
ModuleNotFoundError: No module named 'requests'

$ pip install requests
# Collecting requests ... 及传递依赖 charset-normalizer, idna, urllib3, certifi 等
```

```python
import requests
response = requests.get("https://missing.csail.mit.edu")
```

**作用：** 说明第三方库需经包管理器安装；安装过程含平台制品选择与传递依赖解析。

### venv 隔离

```bash
$ which python
/usr/bin/python
$ python -m venv venv
$ source venv/bin/activate
$ which python
/home/missingsemester/venv/bin/python
$ which pip
/home/missingsemester/venv/bin/pip
$ pip list
# 仅有 pip 等，与全局隔离
```

**作用：** 为项目创建独立 Python 与 site-packages，避免污染系统 Python、化解依赖冲突。

### uv 加速安装与多版本 venv

```bash
$ pip install uv
$ uv pip install requests
Resolved 5 packages in 12ms
...

$ uv venv --python 3.12 venv312
$ source venv312/bin/activate && python --version
Python 3.12.7

$ uv venv --python 3.11 venv311
```

**作用：** `uv pip` 接口类似 pip 但快得多；`uv venv --python` 按项目需要固定不同 CPython 版本。

### 未打包模块换目录失败

```bash
$ cat greet.py
def greet(name):
    print(f"Hello, {name}!")

$ python -c "from greet import greet; greet('World')"
Hello, World!

$ cd /tmp
$ python -c "from greet import greet; greet('World')"
ModuleNotFoundError: No module named 'greet'
```

**作用：** 说明仅靠当前目录导入不可移植；需要打包安装到环境已知路径。

### pyproject.toml + greeting CLI

```toml
[project]
name = "greeting"
version = "0.1.0"
description = "A simple greeting library"
dependencies = ["typer>=0.9"]

[project.scripts]
greet = "greeting:cli"

[build-system]
requires = ["setuptools>=61.0"]
build-backend = "setuptools.build_meta"
```

```python
import typer

def greet(name: str) -> None:
    print(f"Hello, {name}!")

def cli() -> None:
    typer.run(greet)

if __name__ == "__main__":
    cli()
```

```bash
$ uv build
# → dist/greeting-0.1.0-py3-none-any.whl 与 .tar.gz
$ unzip -l dist/greeting-0.1.0-py3-none-any.whl
$ uv pip install ./greeting-0.1.0-py3-none-any.whl
$ greet Alice
Hello, Alice!
```

**作用：** 用现代清单声明元数据与 CLI 入口；构建 wheel/sdist；安装后环境任意目录可用 `greet` 命令。

### SemVer 约束写法（PEP 440）

```toml
[project]
dependencies = [
    "requests==2.32.3",  # Exact version
    "click>=8.0",        # Minimum version
    "numpy>=1.24,<2.0",  # Range
    "pandas~=2.1.0",     # Compatible release: >=2.1.0,<2.2.0
]
```

**作用：** 精确钉死、下限、区间与 `~=` 兼容发布；`~=` 大致类似 npm/cargo 的 `^` 语义。

### Lockfile

```bash
$ uv lock
Resolved 12 packages in 45ms

$ cat uv.lock | head -20
version = 1
requires-python = ">=3.11"
[[package]]
name = "certifi"
version = "2024.8.30"
...
```

**作用：** 把求解出的精确依赖图（含哈希/URL）固化，保证他人/CI 装到同一组版本。

### Docker 运行与 Dockerfile

```bash
$ docker run -it python:3.12 python
>>> print("Hello from inside a container!")
```

```dockerfile
# 讲义中有问题的示例（过多层、未钉版本等）
FROM python:3.12
RUN apt-get update
RUN apt-get install -y gcc
RUN apt-get install -y libpq-dev
RUN pip install numpy
RUN pip install pandas
COPY . /app
WORKDIR /app
RUN pip install .
```

```dockerfile
# 改进版：slim、合并 apt、清缓存、uv sync --locked、builder 拷贝 uv
FROM python:3.12-slim
COPY --from=ghcr.io/astral-sh/uv:latest /uv /usr/local/bin/uv
RUN apt-get update && \
    apt-get install -y --no-install-recommends gcc libpq-dev && \
    rm -rf /var/lib/apt/lists/*
WORKDIR /app
ENV PATH="/app/.venv/bin:$PATH"
COPY pyproject.toml uv.lock ./
RUN uv sync --locked --no-dev --no-install-project
COPY . .
RUN uv sync --locked --no-dev
```

**作用：** 用容器固化运行环境；改进 Dockerfile 减小镜像、可复现安装，并用 builder pattern 只带入所需二进制。

### 环境变量与 YAML 配置

```python
import os

DATABASE_URL = os.environ.get("DATABASE_URL", "sqlite:///local.db")
DEBUG = os.environ.get("DEBUG", "false").lower() == "true"
API_KEY = os.environ["API_KEY"]  # Required - will raise if not set
```

```yaml
database:
  url: "postgresql://localhost/myapp"
  pool_size: 5
server:
  host: "0.0.0.0"
  port: 8080
  debug: false
```

**作用：** 运行时注入配置；可选有默认值，必需密钥用直接索引以便缺失时失败；同一代码多环境只改配置。

### Docker Compose 多服务

```yaml
# docker-compose.yml
services:
  web:
    build: .
    ports:
      - "8080:8080"
    environment:
      - REDIS_URL=redis://cache:6379
    depends_on:
      - cache

  cache:
    image: redis:7-alpine
    volumes:
      - redis_data:/data

volumes:
  redis_data:
```

**作用：** 声明式编排应用与 Redis；服务名 `cache` 作内部 DNS；卷持久化数据。

### systemd 托管 compose

```ini
# /etc/systemd/system/myapp.service
[Unit]
Description=My Application
Requires=docker.service
After=docker.service

[Service]
Type=oneshot
RemainAfterExit=yes
WorkingDirectory=/opt/myapp
ExecStart=/usr/bin/docker compose up -d
ExecStop=/usr/bin/docker compose down

[Install]
WantedBy=multi-user.target
```

**作用：** 开机在 Docker 就绪后拉起 compose；用 `systemctl start/stop/status` 管理。

### 调用外部 HTTP API

```bash
$ curl https://api.anthropic.com/v1/messages \
    -H "x-api-key: $ANTHROPIC_API_KEY" \
    -H "content-type: application/json" \
    -H "anthropic-version: 2023-06-01" \
    -d '{"model": "claude-sonnet-4-20250514", "max_tokens": 256,
         "messages": [{"role": "user", "content": "Explain containers vs VMs in one sentence."}]}'
```

**作用：** 演示服务经标准 HTTP API 协作，而非同进程硬绑定。

### 从 Git / Release / PyPI 安装与发布

```bash
$ pip install git+https://github.com/psf/requests.git
$ pip install git+https://github.com/psf/requests.git@v2.32.3
$ pip install https://github.com/user/repo/releases/download/v1.0/package-1.0-py3-none-any.whl

$ uv pip install requests --verbose --no-cache 2>&1 | grep -F '.whl'
# 可见 py3-none-any 等通用 wheel

$ uv pip install numpy --verbose --no-cache 2>&1 | grep -F '.whl'
# 可见 cp312-...-macosx_..._arm64 等平台 wheel

$ uv publish --publish-url https://test.pypi.org/legacy/
$ uv pip install --index-url https://test.pypi.org/simple/ greeting
```

**作用：** 多种分发/安装路径；理解 wheel 标签与平台相关；用 TestPyPI 安全练习发布。

---

## 练习要点

1. **`printenv` 对比 venv**：激活前后 diff；看 `$PATH` 为何优先 venv；`which deactivate` 理解 deactivate 函数。
2. **写 `pyproject.toml` 包**：装进 venv；生成并查看 lockfile。
3. **Docker Compose**：本地用 compose 构建 Missing Semester 课程网站。
4. **Dockerfile + compose**：简单 Python 应用 + Redis。
5. **TestPyPI 发布**（勿无意义刷正式 PyPI）；用该包装 Docker 镜像并推到 `ghcr.io`。
6. **GitHub Pages** 建站；可选自定义域名。

---

## 延伸阅读

- 原讲义：https://missing.csail.mit.edu/2026/shipping-code/
- [Semantic Versioning](https://semver.org/) 完整规范
- PEP 440（Python 版本说明符）、PEP 517 / PEP 621（构建与 `pyproject.toml`）
- Nix / Bazel（hermetic builds）；Dependabot 类依赖更新；后续「Code Quality and CI」讲次中的 CI
- Docker / containerd / Kubernetes 文档；Docker Hub、PyPI、TestPyPI、GitHub Releases / Pages
