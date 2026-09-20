# fakengine

[![CI](https://github.com/esrrhs/fakengine/actions/workflows/ci.yml/badge.svg)](https://github.com/esrrhs/fakengine/actions/workflows/ci.yml)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20(MinGW)-lightgrey.svg)
![Architecture](https://img.shields.io/badge/arch-x86--64-brightgreen.svg)

**fakengine** 是一个使用现代 C++17 编写的高性能游戏服务器引擎核心库。

---

## 核心设计理念

- **Header-Only 架构**：主体模块采用 Header-Only 规范组织，免除繁杂的静态库/动态库依赖链接流程，即引即用。
- **内存预分配（Zero-Allocation at Runtime）**：所有基础容器、字符串与队列支持定长预分配模式，杜绝运行时内存碎片产生，降低系统调用与 GC 抖动。
- **全面 64 位现代化**：全面原生适配 64 位操作系统（x86-64 Linux 与 Windows MinGW-w64），消除跨平台指针与宽度截断隐患。
- **高性能无锁/轻量网络模型**：内置基于 Epoll (Linux) 与 Select (Windows) 的统一事件处理器、跨平台共享内存与高性能双端网络通信。

---

## 核心模块概览

```
fakengine/
├── 基础数据结构与容器
│   ├── farray.h             # 预分配定长数组
│   ├── flist.h              # 高性能双向链表
│   ├── fhashmap.h / fhashset.h  # 高性能哈希映射与集合
│   ├── fpool.h              # 定长对象缓冲池
│   ├── circle_buffer.h      # 环形无锁/高性能循环队列
│   └── fstack.h             # 定长栈容器
│
├── 高性能字符串
│   └── fstring.h            # 预分配安全定长字符串（全量查找、替换、格式化支持）
│
├── 网络通信框架
│   ├── tcpsocket.h          # 跨平台 TCP 套接字封装（非阻塞、收发缓冲）
│   ├── epollor.h            # Linux epoll / Windows select 统一事件多路复用
│   ├── socket_container.h   # 多连接并发套接字容器
│   ├── socket_link.h        # 客户端点对点链路
│   ├── netmsg.h / proxymsg.h# 网络消息帧结构与序列化协议
│   └── fproxyclient.h       # 代理网关客户端链路管理
│
├── 内存与系统抽象
│   ├── sharemm.h            # 跨平台 64 位共享内存管理
│   ├── calloc.h             # 快速分级定长内存预分配池
│   ├── allocator.h          # STL 兼容的标准分配器适配
│   ├── thread_lock.h        # 互斥锁、自旋锁与自动锁
│   └── fparallel.h          # 原子操作（CAS、Atomic Fetch & Add）
│
├── 加密与算法
│   ├── fmd5.h / fsha1.h     # MD5 与 SHA1 哈希校验
│   ├── fcrc32.h             # 高速 CRC32 计算
│   ├── faes.h / fdes.h      # 对称加密算法
│   └── ftrie.h              # 字典树（高性能敏感词检索与过滤）
│
├── 3D 几何数学（Dimension）
│   ├── vector3d.h           # 3 维空间向量运算
│   ├── plane3d.h            # 3 维空间平面方程与相交判定
│   └── triangle3d.h         # 空间三角形与四边形几何测试
│
└── 系统工具与调试
    ├── inifile.h            # INI 格式配置文件解析器
    ├── fclock.h / ftime.h   # 跨平台单调时钟与纳秒级时间戳
    ├── flog.h               # 控制台/文件日志分级输出
    ├── fhook.h              # 64 位机器码入口 Hook
    └── fptrace.h            # Linux 64 位系统调用监控追踪
```

---

## 配套自动化工具

项目在 `tools/` 目录下提供了一系列原生 64 位的代码生成与协议维护工具：

| 工具目录 | 说明 | 架构平台 |
| :--- | :--- | :--- |
| **`tools/cmdcontroller`** | 跨平台进程命令行发送与控制器 | C++17 原生编译 |
| **`tools/shmclean`** | 共享内存残留自动清理工具 | C++17 原生编译 |
| **`tools/fproxy`** | 服务器网络代理与中继服务组件 | C++17 原生编译 |
| **`tools/gencfg`** | 结构化配置数据解析与 C++ 加载代码生成器 | 64 位 ELF / PE32+ |
| **`tools/genmsg`** | 网络消息 XML 描述到 C++ 序列化代码生成器 | 64 位 ELF / PE32+ |
| **`tools/genstat`** | 性能与业务指标统计代码生成器 | 64 位 ELF / PE32+ |
| **`tools/wireshark`** | 自定义游戏私有网络协议的 Wireshark 抓包插件 | C 原生源码 |

---

## 构建与测试

### 环境要求
- **编译器**：支持 C++17 的 GCC 9+、Clang 10+ 或 Windows MinGW-w64 (x86-64)
- **构建系统**：CMake 3.16+
- **系统库依赖**：`zlib`（系统自带或包管理器安装）、`pthread`、`dl`、`rt`
- **单元测试依赖**：`GoogleTest`（可选；若系统未安装将自动通过 CMake FetchContent 下载编译）

---

### 快速一键构建（推荐）

项目根目录提供了便捷的构建脚本，集配置、编译、共享内存清理、CTest 回归测试及 GoogleTest 验证于一体：

```bash
# Debug 模式构建并运行全部单测
./build.sh

# Release 模式构建并运行全部单测
./build.sh release
```

---

### 标准 CMake 构建

```bash
# 1. 生成构建目录（可指定 Debug 或 Release）
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. 并行编译所有目标（引擎核心、工具与测试程序）
cmake --build build -j$(nproc)

# 3. 运行自动化测试套件
ctest --test-dir build --output-on-failure

# 4. 单独运行现代 GoogleTest 套件查看细项输出
./bin/unit_tests
```

---

## 持续集成 (CI)

本项目配置了自动化 **GitHub Actions CI**（配置文件位于 [`.github/workflows/ci.yml`](.github/workflows/ci.yml)），每次提交与合并均会在多系统与多模式下自动触发交叉验证：

- **Linux (Ubuntu 最新版)**：GCC Debug / Release 双矩阵构建与 100% 测试执行。
- **Windows (MinGW-w64 x86-64)**：MSYS2 环境下 Debug / Release 双矩阵构建与回归测试。
