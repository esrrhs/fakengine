fakengine
======================================
**fakengine**是c++编写的游戏服务器引擎
# 特点 #
* 大部分纯头文件
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
