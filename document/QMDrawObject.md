# QMDrawObject
***

绘制对象抽象基类。所有自定义绘制对象必须继承此类并实现纯虚函数。

## 构造函数

### `~QMDrawObject()`
虚析构函数
***

## 纯虚方法（子类必须实现）

### `virtual void draw(QPainter* painter) = 0`
执行绘制操作  
*painter* — QPainter 绘制对象，用于执行具体的绘制命令
***

### `virtual QPen* pen() const = 0`
获取绘制使用的画笔  
**Returns** — 画笔指针
***

### `virtual QBrush* brush() const = 0`
获取绘制使用的画刷  
**Returns** — 画刷指针
***

## 虚方法（子类可重写）

### `virtual void begin(QPoint point)`
开始绘制操作，设置 isRecord_ 为 true 并记录起始点  
*point* — 绘制起始点坐标
***

### `virtual void end(QPoint point)`
结束绘制操作，设置 isRecord_ 为 false 并记录结束点  
*point* — 绘制结束点坐标
***

### `virtual void recordPoint(QPoint point)`
记录绘制过程中的中间点，仅在 isRecord_ 为 true 时生效  
*point* — 需要记录的中间点坐标
***

## 保护成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `isRecord_` | `bool` | 是否处于记录状态，默认为 false |
| `begin_` | `QPoint` | 绘制起始点 |
| `end_` | `QPoint` | 绘制结束点 |
| `record_` | `QPoint` | 当前记录点位置 |

***
