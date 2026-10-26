# 十五数码问题：C++ A* 第一版

本项目使用 **A* 搜索和曼哈顿距离启发函数**，求解一个简单的十五数码局面。采用多文件 C++17 实现，仅依赖 C++ 标准库。

本版包含一个固定示例、完整解路径输出和本说明；暂不包含复杂实验、性能分析、图形界面或 Word 报告。

## 1. 项目结构

| 文件 | 作用 |
| --- | --- |
| `main.cpp` | 设置初始棋盘和目标棋盘，调用搜索，输出求解过程 |
| `puzzle.h` | 声明棋盘类型、移动方向和棋盘相关函数 |
| `puzzle.cpp` | 实现合法移动、曼哈顿距离、棋盘检查和打印 |
| `astar.h` | 声明搜索接口和结果结构 |
| `astar.cpp` | 实现 A* 搜索和路径恢复 |
| `CMakeLists.txt` | 可选的 CMake 构建配置 |
| `README.md` | 项目说明与运行方法 |

建议按 `main.cpp → puzzle.h → puzzle.cpp → astar.h → astar.cpp` 的顺序阅读。

## 2. 棋盘与示例

棋盘用 `std::array<int, 16>` 按行存储，`0` 表示空格，输出时显示为 `.`。上下左右操作均指**空格**的移动方向，每次移动代价为 1。

初始棋盘：

```text
  1  2  3  4
  5  .  6  8
  9 10  7 11
 13 14 15 12
```

目标棋盘：

```text
  1  2  3  4
  5  6  7  8
  9 10 11 12
 13 14 15  .
```

该示例的解为 **Right → Down → Right → Down**，即空格依次向右、向下、向右、向下移动，共 4 步。程序会打印包括初始局面在内的全部 5 个棋盘。

如需更换示例，修改 `main.cpp` 中的 `start` 数组，再重新编译。每个棋盘必须恰好包含一次 0 到 15。

## 3. 算法思路

A* 使用以下估价函数决定先搜索哪个状态：

```text
f(n) = g(n) + h(n)
```

- `g(n)`：从初始棋盘走到当前棋盘的已知最少步数。
- `h(n)`：所有非空数字到各自目标位置的曼哈顿距离之和。
- `f(n)`：经过当前棋盘到达目标的估计总步数。

一个数字的曼哈顿距离为：

```text
abs(当前行 - 目标行) + abs(当前列 - 目标列)
```

空格不计入距离。每次合法移动只会让一个数字移动一格，因此该距离和不会高估实际剩余步数；对于相邻棋盘，它的变化不超过 1，满足一致性。采用此启发函数的标准 A* 在成功结束时可以得到最短解。

本示例中数字 6、7、11、12 各偏离目标一格，初始曼哈顿距离为 4，因此至少需要 4 步；上面的路径恰好用 4 步到达目标。

### 搜索过程

1. 将初始棋盘放入 OPEN 优先队列，初始 `g` 为 0。
2. 取出 `f` 最小的棋盘；若 `f` 相同，优先取 `h` 较小的棋盘。
3. 跳过已经失效或已经扩展的记录。
4. 如果取出的棋盘是目标棋盘，沿父节点记录恢复解路径。
5. 否则将其放入 CLOSED 集合，生成空格可以合法移动到的相邻棋盘。
6. 如果到达某个相邻棋盘的新路径更短，更新它的步数、父节点和移动方向，并加入 OPEN。
7. 重复以上步骤。

`gScore` 用来记录已知最小步数，`parents` 用来恢复路径。优先队列不直接修改旧元素，而是加入更新后的记录，在取出时跳过旧记录。由于此处曼哈顿距离具有一致性，CLOSED 中的状态无需重新打开。

### 基本输入检查

搜索前检查数字是否齐全、是否重复，并比较起点和终点的可解性。对 4×4 棋盘，去掉空格后计算逆序数，再加上空格自底向上的行号（从 1 开始）；起点与终点的这个和必须具有相同奇偶性。

这是为了在修改示例时及时识别非法或不可解输入。本版面向简单局面，未加入搜索资源上限；打乱程度很高的局面可能占用较多时间和内存。

## 4. 编译与运行

以下方式任选一种。命令均从本项目目录执行，须将三个 `.cpp` 一起编译，不能只编译 `main.cpp`。

### 方式 A：Visual Studio 的 C++ 编译器（Windows）

打开 **x64 Native Tools Command Prompt for VS 2022**，执行：

```bat
cd /d E:\Desktop\MyCode\AIHomework
if not exist build mkdir build
cd build
cl /nologo /std:c++17 /EHsc /W4 /utf-8 /O2 ..\main.cpp ..\puzzle.cpp ..\astar.cpp /Fe:fifteen_puzzle.exe
fifteen_puzzle.exe
```

普通 PowerShell 默认可能找不到 `cl`，需要使用上述开发者命令提示符，让 Visual Studio 配置编译器环境。

### 方式 B：g++

已安装支持 C++17 的 g++ 时，可在项目目录下执行：

```text
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic main.cpp puzzle.cpp astar.cpp -o fifteen_puzzle.exe
```

Windows PowerShell 中运行：

```powershell
.\fifteen_puzzle.exe
```

### 方式 C：CMake + Visual Studio 2022

如果已安装 CMake 和 Visual Studio 的 C++ 开发工具，可执行：

```text
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Windows PowerShell 中运行：

```powershell
.\build\Release\fifteen_puzzle.exe
```

## 5. 示例输出

关键输出如下，后面还会逐步打印棋盘：

```text
Solved! Minimum moves: 4
Moves: Right Down Right Down
```

方向文字采用英文，避免不同 Windows 控制台编码影响显示；源代码中的解释性注释和本文档使用中文。

### 本次运行检查

已在本机使用 Visual Studio 2022 的 MSVC 14.44 编译器，以 C++17、`/W4` 配置完成编译，无编译警告。运行内置示例后正常退出，输出 4 步路径 `Right Down Right Down`；逐步核对了空格交换过程，最后棋盘与目标一致。

本次只做了上述编译和示例运行检查，尚未运行复杂局面、性能实验或系统性测试；g++ 和 CMake 构建方式尚未在本机验证。

## 6. 资料依据

- `2026年人工智能第一次大作业要求.pdf`：采用 A* 解决十五数码问题，允许自定义可行的初始状态和目标状态。
- `对应内容课程讲义.pdf`：状态空间、OPEN/CLOSED、估价函数、启发式搜索和 A*。

本版使用上面的简单自定义棋盘，不使用 `十五数码问题.png` 中较复杂的初始局面。
