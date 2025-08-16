# Unity CLI 工具

一个用于管理Unity项目的C++命令行工具，提供项目清理和目录结构创建功能。

## 功能特性

1. **清理功能** - 删除Unity项目中不必要的文件和目录
2. **目录结构创建** - 根据YAML模板在Assets目录下创建文件夹结构
3. **可扩展架构** - 易于添加新的命令和功能

## 构建要求

- C++20兼容的编译器
- CMake 3.16或更高版本
- 文件系统库支持

## 构建步骤

```bash
cd build
cmake ..
make
```

## 使用方法

### 清理Unity项目

```bash
./unity-cli clean /path/to/unity/project
```

此命令会删除以下内容：
- `Library/` 目录
- `Temp/` 目录
- `obj/` 目录
- `Logs/` 目录
- `.vs/`, `.vscode/` 目录
- `Build/`, `Builds/` 目录
- 各种临时文件和IDE生成的文件

### 创建目录结构

```bash
./unity-cli structure /path/to/unity/project resource/template_example.yaml
```

根据YAML模板在项目的Assets目录下创建文件夹结构。可以使用项目提供的示例模板或创建自定义模板。

### YAML模板格式

```yaml
# 使用2个空格表示缩进层级
# 以'/'结尾的表示目录，包含'.'的表示文件

Scripts/
  Player/
    PlayerController.cs
    PlayerMovement.cs
  UI/
    UIManager.cs

Prefabs/
  Player/
  UI/

Materials/
  Player/
  Environment/
```

## 可扩展性

要添加新的命令：

1. 创建继承自 `CommandInterface` 的新类
2. 实现 `execute()` 和 `getDescription()` 方法
3. 在 `main.cpp` 中注册新命令

```cpp
manager.registerCommand("newcommand", std::make_unique<NewCommand>());
```

## 目录结构

```
unity-cli/
├── build/                   # 编译输出目录
├── src/                     # 源文件目录
│   ├── main.cpp            # 主程序入口
│   ├── command_manager.cpp # 命令管理器实现
│   ├── clean_command.cpp   # 清理命令实现
│   └── structure_command.cpp # 目录结构命令实现
├── include/                 # 头文件目录
│   ├── command_interface.h # 命令接口定义
│   ├── command_manager.h   # 命令管理器头文件
│   ├── clean_command.h     # 清理命令头文件
│   └── structure_command.h # 目录结构命令头文件
├── resource/                # 资源文件目录
│   └── template_example.yaml # YAML模板示例
├── CMakeLists.txt          # 构建配置
└── README.md               # 说明文档
```

## 许可证

本项目使用MIT许可证。