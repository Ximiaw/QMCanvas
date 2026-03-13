# QMCanvas
***

基于 Qt6 的轻量级图形编辑画布组件，提供类似图像编辑软件的画布交互体验。

> ⚠️ **注意：这是一个学习项目，代码功能尚未经过完整测试，仅供学习参考使用。**

## 架构概览
***

QMCanvas 采用分层架构设计，各组件职责分离：

```
┌─────────────────────────────────────────┐
│           QMCanvasView                  │  ← 对外接口容器
│  ┌─────────────────────────────────┐    │
│  │              View               │    │  ← QScrollArea，处理滚动和滚轮
│  │  ┌─────────────────────────┐    │    │
│  │  │        Viewport         │    │    │  ← 渲染部件，处理鼠标事件
│  │  │  ┌─────────────────┐    │    │    │
│  │  │  │  QMCanvasScene  │    │    │    │  ← 场景管理器
│  │  │  │  ┌───────────┐  │    │    │    │
│  │  │  │  │  QPixmap  │  │    │    │    │  ← 位图数据
│  │  │  │  └───────────┘  │    │    │    │
│  │  │  │  ┌───────────┐  │    │    │    │
│  │  │  │  │QMDrawObject│  │    │    │    │  ← 绘制对象列表
│  │  │  │  └───────────┘  │    │    │    │
│  │  │  └─────────────────┘    │    │    │
│  │  └─────────────────────────┘    │    │
│  └─────────────────────────────────┘    │
└─────────────────────────────────────────┘
```

## 核心组件
***

| 组件 | 继承自 | 职责 |
|------|--------|------|
| `QMCanvasView` | `QWidget` | 对外接口容器，整合 View 和 Viewport |
| `View` | `QScrollArea` | 滚动视图，处理滚轮事件（滚动/缩放模式切换） |
| `Viewport` | `QWidget` | 实际渲染区域，转发鼠标事件 |
| `QMCanvasScene` | `QObject` | 场景管理器，维护位图数据和绘制对象列表 |
| `QMDrawObject` | `QObject` | 绘制对象抽象基类 |

## 快速开始
***

### 基础用法

```cpp
#include "QMCanvasView.h"
#include "QMCanvasScene.h"

// 创建画布视图
QMCanvasView* canvasView = new QMCanvasView(parent);

// 创建场景（传入初始位图）
QPixmap pixmap(800, 600);
pixmap.fill(Qt::white);
QMCanvasScene* scene = new QMCanvasScene(pixmap);

// 关联场景
canvasView->setCanvasScene(scene);

// 设置滚轮模式为缩放
canvasView->setWheelMode(WheelMode::ZOOM);
```

### 实现自定义绘图工具

继承 `QMDrawObject` 实现纯虚函数：

```cpp
class MyPen : public QMDrawObject {
public:
    void begin(QPoint point) override {
        points_.clear();
        points_.append(point);
    }
    
    void end(QPoint point) override {
        points_.append(point);
    }
    
    void recordPoint(QPoint point) override {
        points_.append(point);
    }
    
    void draw(QPainter* painter) override {
        painter->setPen(QPen(Qt::black, 2));
        for (int i = 1; i < points_.size(); ++i) {
            painter->drawLine(points_[i-1], points_[i]);
        }
    }
    
    QPen* pen() const override { return &pen_; }
    QBrush* brush() const override { return &brush_; }
    
private:
    QVector<QPoint> points_;
    QPen pen_;
    QBrush brush_;
};

// 使用绘图工具
scene->setActiveDrawObject(new MyPen(scene));
```

## 依赖
***

- Qt6 (Core, Gui, Widgets)
- C++17 或更高版本

## 已知问题
***

- 多线程图像合成功能尚未实现（代码中有注释掉的逻辑）
- 复杂变换下的坐标精度未充分验证
- 大量绘图对象时的性能优化待完善
- 边界情况下的滚动条同步可能存在异常  
......

## 作者
***

Created by Ximiaw (2026)
