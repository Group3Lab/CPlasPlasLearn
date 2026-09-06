# C & Python 练习工程

同时练习 **C 语言**（MinGW-w64 GCC 8.1.0）和 **Python**（conda 环境 `py_practice`）。

## 项目结构

```
C_learning/
├── .vscode/
│   ├── settings.json   # 指定 Python 解释器 = conda py_practice
│   ├── tasks.json      # C 编译任务
│   └── launch.json     # C (GDB) + Python (debugpy) 调试配置
├── exercises/          # C 源码目录（只放 .c）
│   └── 01_hello_world.c
├── build/              # C 编译产物（.exe）
├── python_exercises/   # Python 练习目录（.py）
│   ├── 01_hello.py
│   └── 02_libs_check.py
└── README.md
```

---

# Part 1 · C 语言练习

编译产物会自动输出到 **`build/`**，源码和 exe 分开放。

### 编译当前 .c
按 **Ctrl + Shift + B** → 选 **"C: 编译当前文件"**。
产物：`./build/<文件名>.exe`

### 一键编译 + 运行
按 **Ctrl + Shift + B** → 选 **"C: 编译并运行当前文件"**。

### 手动编译
```bash
gcc -Wall -g exercises/01_hello_world.c -o build/01_hello_world.exe
```

### C 调试
在 `.c` 中打断点，按 **F5**（选 "C: GDB"），会先自动编译再进入调试。

---

# Part 2 · Python 练习

Python 练习使用独立的 conda 环境 **`py_practice`**（Python 3.12，已装 numpy/pandas/matplotlib），不会影响你其他专用环境。

### 运行 .py 文件
- **最简单**：打开 `.py`，点编辑器右上角 ▶ "运行 Python 文件"
- 快捷键运行/调试：打开 `.py` 后按 **F5**（用 debugpy）
- 手动运行某文件：
  ```bash
  conda run -n py_practice python python_exercises/01_hello.py
  ```

### 加入新库
```bash
conda run -n py_practice pip install <库名>
```

### 命名建议
与 C 类似，数字递增 + 语义：`01_hello.py`、`02_if_else.py`、`03_functions.py` …

---

## Python 环境小贴士
- 中文输出**不会乱码**（Python 默认 UTF-8），区别于 C 的控制台乱码问题。
- 本机该环境路径为 `D:\Software\Miniconda\envs\py_practice`，已在 `.vscode/settings.json` 设为默认解释器。
- `.vscode/settings.json` 仅存本机，未被提交（`.gitignore` 忽略 `.vscode/`），换电脑需自行重设解释器。
