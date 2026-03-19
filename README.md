# QMCanvas

一个基于 Qt6 的简易画布组件，用于学习和理解图像渲染、图层管理和视口变换的基本概念。

> **注意**：这是一个学习项目，代码未经充分测试，仅供学习参考。

## 项目概述

QMCanvas 实现了一个支持缩放、滚动、图层管理的画布系统。它提供了基础的图像显示和交互功能。

### 主要特性

- **视口渲染**：支持视口外扩预渲染，优化滚动体验
- **图层管理**：多层图层系统，支持图层切换和隐藏控制
- **撤销/重做**：基于栈的撤销重做机制
- **缩放控制**：支持鼠标滚轮缩放，可配置缩放倍率范围
- **双模式滚轮**：滚轮可在滚动视图和缩放视图两种模式间切换
- **Ctrl 键快捷缩放**：按住 Ctrl 键时滚轮自动切换为缩放模式

## 架构说明

```
┌─────────────────────────────────────────────────────────────┐
│                      1. 视图层                              │
│  ┌─────────────┐  ┌─────────────┐  ┌───────────────────┐    │
│  │QMCanvasView │  │    View     │  │     Viewport      │    │
│  │   顶层容器  │  │ QScrollArea │  │    实际渲染部件    │    │
│  └─────────────┘  └─────────────┘  └───────────────────┘    │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      2. 场景层                              │
│               ┌─────────────────────┐                       │
│               │   QMCanvasScene     │                       │
│               │   场景管理&协调中心 │                        │
│               └─────────────────────┘                       │
└─────────────────────────────────────────────────────────────┘
                              │
┌─────────────────────────────────────────────────────────────┐
│                      3. 管理层                              │
│  ┌─────────────┐  ┌─────────────┐  ┌───────────────────┐    │
│  │  Location   │  │ LayerManager│  │      Layer        │    │
│  │ 坐标变换管理│  │  多图层合成  │  │    单图层容器     │    │
│  └─────────────┘  └─────────────┘  └───────────────────┘    │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      4. 绘制层                              │
│  ┌─────────────┐  ┌─────────────┐  ┌───────────────────┐    │
│  │QMDrawObject │  │  QMDrawPen  │  │    QMDrawRect     │    │
│  │   抽象基类  │  │   手绘线条   │ │     矩形绘制       │    │
│  └─────────────┘  └─────────────┘  └───────────────────┘    │
│  ┌─────────────┐                                            │
│  │ QMDrawPixmap│                                            │
│  │   位图渲染  │                                            │
│  └─────────────┘                                            │
└─────────────────────────────────────────────────────────────┘
```

### 核心组件

| 组件                | 说明                             |
| ----------------- | ------------------------------ |
| **QMCanvasView**  | 顶层容器，整合 View 和 Viewport，提供统一接口 |
| **View**          | 基于 QScrollArea 的滚动视图，处理滚轮事件    |
| **Viewport**      | 实际渲染部件，负责绘制和鼠标事件转发             |
| **QMCanvasScene** | 场景管理器，协调位置计算、图层管理和渲染           |
| **Location**      | 位置管理器，处理坐标变换、缩放和视口计算           |
| **LayerManager**  | 图层管理器，管理多个图层的合成渲染              |
| **Layer**         | 单个图层，管理该图层内的绘制对象               |
| **QMDrawObject**  | 绘制对象抽象基类，所有可绘制对象需继承此类          |
| **QMDrawPixmap**  | 位图绘制对象，用于绘制 QPixmap，支持填充/非填充模式 |
| **QMDrawPen**     | 手绘线绘制对象，支持多点连续线条绘制             |
| **QMDrawRect**    | 矩形绘制对象，支持描边和填充样式               |


### 数据流

```
鼠标/键盘事件 → Viewport → QMCanvasScene → Location/LayerManager
                                              ↓
        重绘显示 ←  Viewport ←  信号通知  ←  更新完成
```

## 构建说明

### 环境要求

- Qt 6.x
- CMake 3.5+
- MinGW (支持 C++20)
- Ninja

### 示例构建步骤

1. 克隆或下载项目源码：
```bash
git clone https://github.com/Ximiaw/QMCanvas.git
cd QMCanvas
```

2. 修改主 CMakeLists.txt 中的 Qt 路径：

打开项目根目录的 `CMakeLists.txt`，将以下行修改为你的 Qt6 安装路径：

```cmake
set(CMAKE_PREFIX_PATH "你的Qt6安装路径")
```

3. 在项目根目录创建构建目录并编译(请配好CMake,MinGW,Ninja的系统变量)：

```bash
cmake -G Ninja -S ./ -B ./build/
cmake --build ./build/
```

4. 运行：

```bash
./build/Demo.exe
```

## 使用示例

```cpp
#include "QMCanvasView.h"
#include "QMCanvasScene.h"
#include <QApplication>
#include <QPixmap>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 创建画布视图
    QMCanvasView canvasView;
    
    // 创建底图
    QPixmap pixmap(1000, 1000);
    pixmap.fill(Qt::yellow);
    
    // 创建场景并设置底图
    QMCanvasScene* scene = new QMCanvasScene(pixmap);
    canvasView.setCanvasScene(scene);
    
    // 设置视图大小
    canvasView.resize(500, 500);
    
    // 配置缩放参数
    canvasView.canvasScene()->setFactor(1.2);   // 每次缩放 20%
    canvasView.canvasScene()->setMaxRatio(5.0); // 最大放大 5 倍
    canvasView.canvasScene()->setMinRatio(0.1); // 最小缩小到 0.1 倍
    
    // 设置滚轮模式为缩放（默认是滚动）
    // canvasView.setWheelMode(WheelMode::ZOOM);
    
    canvasView.show();
    
    return app.exec();
}
```

## 扩展绘制对象

要创建自定义绘制对象，继承 `QMDrawObject` 并实现纯虚方法：

```cpp
class MyDrawObject : public QMDrawObject {
public:
    void draw(QPainter* painter) override {
        // 实现绘制逻辑
        painter->drawLine(begin_, end_);
    }
    
    QPen* pen() override {
        return &pen_;
    }
    
    QBrush* brush() override {
        return &brush_;
    }
    
private:
    QPen pen_;
    QBrush brush_;
};
```

## 类文档

- [AbstractLayer](document/AbstractLayer.md) - 图层管理模板基类
  - [Layer](document/Layer.md) - 图层类
  - [LayerManager](document/LayerManager.md) - 图层管理器
- [Location](document/Location.md) - 位置管理类
- [QMCanvasScene](document/QMCanvasScene.md) - 场景管理器
- [QMCanvasView](document/QMCanvasView.md) - 画布视图容器
- [QMDrawObject](document/QMDrawObject.md) - 绘制对象基类
  - [QMDrawPixmap](document/QMDrawPixmap.md) - 位图绘制对象
  - [QMDrawPen](document/QMDrawPen.md) - 手绘线条对象
  - [QMDrawRect](document/QMDrawRect.md) - 矩形绘制对象
- [View](document/View.md) - 滚动视图
- [Viewport](document/Viewport.md) - 视口渲染部件
- [WheelMode](document/WheelMode.md) - 滚轮模式枚举

## 快捷键

| 快捷键 | 功能 |
|--------|------|
| Ctrl + Z | 撤销 |
| Ctrl + Y | 重做 |
| Ctrl + S | 保存（发送 save() 信号，需自行实现）|
| 按住 Ctrl + 滚轮 | 缩放视图 |

## 许可证

MIT License

Copyright (c) 2026 Ximiaw
