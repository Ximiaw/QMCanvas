# QMDrawPixmap
***

位图绘制对象类，继承自 `QMDrawObject`。用于在位图上绘制另一个位图，支持全图填充或区域绘制模式。

## 构造函数

### `QMDrawPixmap(QPixmap& pixmap)`
初始化位图绘制对象，使用指定的位图  
*pixmap* — 要绘制的位图
***

### `QMDrawPixmap()`
初始化位图绘制对象，创建 500x500 的透明位图
***

## 公共方法

### `bool fill()`
获取当前填充模式  
**Returns** — true 表示全图填充模式，false 表示区域绘制模式
***

### `void setFill(bool fill)`
设置填充模式  
*fill* — true 表示全图填充，false 表示按 begin_/end_ 区域绘制
***

### `void setPixmap(QPixmap& pixmap)`
设置要绘制的位图  
*pixmap* — 新的位图
***

### `void draw(QPainter* painter) override`
执行绘制操作，在 painter 上绘制位图。若 fill_ 为 true 则全图填充，否则按 begin_/end_ 区域绘制  
*painter* — QPainter 绘制对象
***

### `QPen* pen() const override`
获取画笔（本类不使用画笔）  
**Returns** — 始终返回 nullptr
***

### `QBrush* brush() const override`
获取画刷（本类不使用画刷）  
**Returns** — 始终返回 nullptr
***

## 继承的方法

继承自 [QMDrawObject](QMDrawObject.md)：

### `void begin(QPoint point)`
开始绘制操作，记录起始点  
*point* — 绘制起始点坐标
***

### `void end(QPoint point)`
结束绘制操作，记录结束点  
*point* — 绘制结束点坐标
***

### `void recordPoint(QPoint point)`
记录绘制过程中的中间点  
*point* — 中间点坐标
***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `pixmap_` | `QPixmap` | 要绘制的位图 |
| `fill_` | `bool` | 是否全图填充模式 |

## 继承的成员

继承自 QMDrawObject：

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `isRecord_` | `bool` | 是否处于记录状态 |
| `begin_` | `QPoint` | 绘制起始点 |
| `end_` | `QPoint` | 绘制结束点 |
| `record_` | `QPoint` | 当前记录点 |

***
