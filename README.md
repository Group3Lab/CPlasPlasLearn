# C 语言练习工程

用于 C 语言入门练习的项目，使用 MinGW-w64 GCC 8.1.0。

## 项目结构

```
C_learning/
├── .vscode/
│   ├── tasks.json      # 编译任务
│   └── launch.json     # 调试配置 (GDB)
├── exercises/          # 练习代码目录（只放 .c 源码，推荐在此写练习）
│   └── 01_hello_world.c
├── build/              # 编译产物目录（.exe 自动输出到这里，无需手动建）
└── README.md
```

## 日常使用

编译产物 `.exe` 会自动输出到 **`build/` 文件夹**，源码和可执行文件分开放，不会混在一起。

### 编译当前文件（Ctrl + Shift + B）
按 **Ctrl + Shift + B**，选择 **"C: 编译当前文件"**，产物会生成到 `build/`，例如：
```bash
./build/01_hello_world.exe
```

### 一键编译 + 运行
按 **Ctrl + Shift + B**，选择 **"C: 编译并运行当前文件"**：先编译到 `build/`，成功后自动运行。

### 编译语法参考（手动）
```bash
gcc -Wall -g exercises/01_hello_world.c -o build/01_hello_world.exe
```

## 调试
在源码中打断点（行号左侧点一下），按 **F5** 即可启动 GDB 调试。会先用 `-g` 参数自动编译再启动。

## 建议命名规格
练习文件建议按数字递增并带语义名称：
- `01_hello_world.c` — 你好世界
- `02_variables.c` — 变量与类型
- `03_if_else.c` — 条件分支
- ...以此类推

每次练习完如果不需要，可随时删除对应的 `.exe`。
