# sxfViewer

`sxfViewer` 是一个基于 Qt Widgets 的 SXF 动画表（X-sheet）查看与编辑工具，支持打开、浏览、编辑并另存 `.sxf` 文件。

## 功能特性

- 打开并解析 SXF 文件（魔数 `WBSC`，大端序）。
- 表格方式查看帧与列数据（包含 ACTION / CELL 分组表头）。
- 直接编辑单元格内容（如 `○ 12`、`# 3`、`15` 等）。
- 编辑全局属性：
  - 最大帧数、层数、FPS
  - 场次（Scene / Cut）
  - 时间显示格式、每页帧数、标尺间隔
  - 可视项开关（Action/Cell/Dialogue/Sound/Camera/Note/Basic Info）
  - 备注文本
- 编辑列属性（当前支持列可见性标记）。
- 将修改后的数据保存为新的 SXF 文件。

## 项目结构

- `/home/runner/work/sxfViewer/sxfViewer/main.cpp`：程序入口。
- `/home/runner/work/sxfViewer/sxfViewer/sxfviewer.*`：主窗口与交互逻辑。
- `/home/runner/work/sxfViewer/sxfViewer/sxfmodel.*`：表格模型与单元格编辑规则。
- `/home/runner/work/sxfViewer/sxfViewer/sxfmergeheaderview.*`：双层分组合并表头绘制。
- `/home/runner/work/sxfViewer/sxfViewer/sxfprocessor.*`：SXF 二进制读写与数据结构定义。
- `/home/runner/work/sxfViewer/sxfViewer/sxf.md`：SXF 格式字段研究笔记。
- `/home/runner/work/sxfViewer/sxfViewer/test.sxf`：示例文件。

## 构建环境

- CMake >= 3.16
- C++17 编译器
- Qt 5 或 Qt 6（Widgets 模块）

## 构建步骤

在仓库根目录执行：

```bash
cmake -S . -B build
cmake --build build
```

## 运行方式

构建完成后运行可执行文件（名称为 `sxfViewer`），在菜单中使用：

- `File -> Open...` 打开 `.sxf`
- `File -> Save As...` 另存编辑结果

## 注意事项

- 本项目当前使用 Qt GUI，构建机需安装 Qt 开发包并正确配置 `CMAKE_PREFIX_PATH`（或 `Qt5_DIR` / `Qt6_DIR`）。
- 备注文本编码当前优先按 GBK 处理（见 `sxfprocessor.cpp` 的读写逻辑）。
- 部分 SXF 区块（如 Sound / Dialogue / Draw）以保留字段方式读写，具体语义仍在整理中。

## License

This project is licensed under the **Modified BSD License (3-Clause BSD)**.  
See the [LICENSE](LICENSE) file for details.
