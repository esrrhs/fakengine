fakengine
======================================
**fakengine**是c++编写的游戏服务器引擎
# 特点 #
* 基本Header Only
* 采用内存预分配原则

# 包含的模块 #
* 基础数据结构，数组、列表、hashmap等
* 字符串string
* 内存池
* 线程对象，线程锁
* 循环队列
* 共享内存
* 工厂定义
* 命令行解析
* 进程控制
* 系统时间
* 常用哈希加密，如md5、crc32、aes、sha1
* 序列化
* 函数hook
* 日志
* 数学库
* 敏感词
* ini文件读取
* 自定义allocator
* 单件
* 网络通信
* profile

# 包含的第三方库 #
* xml解析库，expat、tinyxml
* 脚本库，lua、fakescript
* 压缩库，lzo、zlib
* 正则库，pcre
* 数据库，mysql、sqlite

# 自动化工具 #
* ini读取代码生成工具
* xml读取代码生成工具
* sqlite读取代码生成工具
* 监控代码生成工具
* 网络消息代码生成工具
* wireshark插件工具

# 编译与测试 #
* 环境要求：C++17 编译器（GCC 8+ / Clang / MSVC），CMake 3.16+
* 持续集成：内置 GitHub Actions CI 流水线（`.github/workflows/ci.yml`），自动进行 Debug / Release 多配置编译与测试验证。
* 单元测试架构：
  - **现代 GoogleTest 单元测试套件**：覆盖基础容器（`farray`, `flist`, `fhashmap`, `fhashset`, `fpool`）、字符串（`fstring`）、加密算法（`fmd5`, `fsha1`, `fcrc32`）、3D 几何数学（`vector3d`, `plane3d`, `triangle3d`, `quadrangle3d`）及解析器（`ftrie`, `inifile`）。
  - **传统应用回归测试**：通过 CTest 进行 14 项完整的功能心跳回归测试。
* 一键构建与测试：
  ```bash
  ./build.sh           # Debug 模式构建、合并库并运行全部单元测试
  ./build.sh release   # Release 模式构建、合并库并运行全部单元测试
  ```
* 现代 CMake 标准构建：
  ```bash
  cmake -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build -j
  ctest --test-dir build --output-on-failure
  ./bin/unit_tests   # 直接运行 GoogleTest 查看详细单测结果
  ```
