# forgotten-lib

[![CI](https://github.com/esrrhs/forgotten-lib/actions/workflows/ci.yml/badge.svg)](https://github.com/esrrhs/forgotten-lib/actions/workflows/ci.yml)
![Type](https://img.shields.io/badge/type-Header--Only-orange.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20(MinGW)-lightgrey.svg)
![Architecture](https://img.shields.io/badge/arch-x86--64-brightgreen.svg)

**forgotten-lib** is a refreshed **header-only C++17 game-server foundation library** (formerly *fakengine*).

Think of it as picking up an old toolbox and cleaning it—not as a pitch for greenfield work. The fixed-size containers, object pools, TCP/`epoll` networking, and heartbeat main loop from an earlier era have been brought up to build cleanly on modern 64-bit Linux and Windows (MinGW-w64). If you were starting an MMO today, you probably would not write the whole server stack in pure C++ from scratch. This project keeps that past toolkit usable; it does not claim to be the modern default.

APIs still use the historical `f*` prefix (`farray` / `fpool` / `fengine` …). Here **f** means the **forgotten** family—not another “engine” brand.

---

## What it is / is not

**Is**

- 100% header-only (CMake `INTERFACE` target, include and go)
- Fixed-capacity containers and object pools (few runtime heap allocations, less jitter)
- Cross-platform non-blocking TCP, `epoll` / `select`, connections and I/O buffers
- Heartbeat main loop, logging, ini, shared memory, and other server-side odds and ends

**Is not**

- A full game engine or turnkey MMO framework (no scene/AOI, persistence, or multi-process story)
- A recommendation for how to build new game servers today

---

## Quick start

### CMake (recommended)

After the GitHub rename, the clone directory is usually `forgotten-lib`. The header tree is still named `fakengine/` for include compatibility:

```cmake
add_subdirectory(forgotten-lib)   # or wherever you placed this repo
# if you kept the old folder name: add_subdirectory(fakengine)

add_executable(my_game_server main.cpp)
target_link_libraries(my_game_server PRIVATE fengine)
```

### Direct includes

Add the header directory to your include path, then:

```cpp
#include "fakengine.h"

int main()
{
    fengine engine("hello");

    farray<int, 1024> arr;
    arr.push_back(42);

    fstring<uint8_t, 128> msg = "hello, forgotten-lib";
    printf("%s\n", (const char *)msg.c_str());
    return 0;
}
```

> Requires **C++17**. Link `pthread`, `dl`, and `rt` on Linux; `ws2_32` on Windows. Optional system `zlib`.

---

## Module overview

```
fakengine/                       # legacy directory name; APIs use f*
├── fakengine.h / fengine.h      # umbrella include and runtime bootstrap
│
├── Containers & strings
│   ├── farray / flist / fstack / fpool
│   ├── fhashmap / fhashset
│   ├── circle_buffer / fkeybuffer
│   └── fstring
│
├── Networking
│   ├── tcpsocket / epollor
│   ├── socket_container / socket_link
│   ├── netmsg / proxymsg / fproxyclient
│   └── netserver / neteventprocessor
│
├── Memory
│   ├── calloc / sharemm
│   ├── normal_allocator / readonly_allocator
│   └── allocator            # STL allocator adapter over the engine heaps
│
├── Misc
│   ├── fclock / ftime       # chrono-based clocks
│   ├── inifile / cmdparser / flog
│   ├── fmd5 / fsha1 / fcrc32 / faes / fdes
│   ├── ftrie / dimension/*
│   └── fprofile / fhook / fptrace
```

---

## Tools

| Path | Purpose |
| :--- | :--- |
| `tools/cmdcontroller` | Send commands into a running process |
| `tools/shmclean` | Clean leftover shared-memory segments |
| `tools/fproxy` | Simple network proxy |
| `tools/gencfg` / `genmsg` / `genstat` | Config / message / stats code generators |
| `tools/wireshark` | Wireshark plugin sources for a private protocol |

---

## Build & test

- Compiler: GCC 9+ / Clang 10+ / MinGW-w64 (x86-64), C++17  
- CMake 3.16+; optional GoogleTest and zlib  

```bash
./build.sh              # Debug + tests
./build.sh release      # Release + tests

# or
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

---

## CI

GitHub Actions ([`.github/workflows/ci.yml`](.github/workflows/ci.yml)):

- Linux GCC: Debug / Release  
- Windows MinGW-w64: Debug / Release  

---

## Naming

| Name | Meaning |
| :--- | :--- |
| **forgotten-lib** | Public project / repository name |
| **`f*` APIs** | Historical prefix; **f** ≈ forgotten |
| **`fakengine/`, `fengine`** | Legacy directory, CMake target, and class names—left as-is to avoid a pointless mass rename; can be cleaned up later |
