# QMDrawPen
***

手绘线条绘制对象类，继承自 `QMDrawObject`。支持记录多点连续绘制自由线条，适用于画笔、涂鸦等手绘操作。

## 构造函数

### `QMDrawPen(QColor color=Qt::black, int width=3)`
初始化手绘线条对象，设置画笔颜色和宽度  
*color* — 线条颜色，默认为黑色  
*width* — 线条宽度（像素），默认为 3 像素
***

## 公共方法

### `void draw(QPainter* painter) override`
执行绘制操作，将记录的所有点连接成线段绘制到 painter 上  
**Note** — 当点数少于 2 个时不会绘制任何内容  
*painter* — QPainter 绘制对象
***

### `void begin(QPoint point) override`
开始绘制操作，记录起始点并添加到点列表  
*point* — 绘制起始点坐标
***

### `void recordPoint(QPoint point) override`
记录绘制过程中的中间点，实时添加到点列表形成线条轨迹  
*point* — 中间点坐标
***

### `void end(QPoint point) override`
结束绘制操作，记录结束点并添加到点列表  
*point* — 绘制结束点坐标
***

### `QPen* pen() override`
获取绘制使用的画笔  
**Returns** — 画笔指针，包含颜色和宽度设置
***

### `QBrush* brush() override`
获取绘制使用的画刷（本类不使用画刷填充）  
**Returns** — 始终返回 nullptr
***

### `QSharedPointer<QMDrawObject> clone() override`
克隆当前绘制对象，创建具有相同画笔属性的新实例  
**Returns** — 新的手绘线条对象智能指针
***

## 继承的方法

继承自 [QMDrawObject](QMDrawObject.md)：

### `bool isRecord() const`
获取当前是否处于记录状态

### `QPoint begin() const`
获取绘制起始点

### `QPoint end() const`
获取绘制结束点

### `QPoint record() const`
获取当前记录点位置

***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `points_` | `QList&lt;QPoint&gt;` | 记录的所有绘制点坐标列表 |
| `pen_` | `QPen` | 画笔属性（颜色、宽度、线型） |

## 继承的成员

继承自 QMDrawObject：

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `isRecord_` | `bool` | 是否处于记录状态 |
| `begin_` | `QPoint` | 绘制起始点 |
| `end_` | `QPoint` | 绘制结束点 |
| `record_` | `QPoint` | 当前记录点 |

***

## 相关类

- [QMDrawObject](QMDrawObject.md) — 绘制对象基类
- [QMDrawRect](QMDrawRect.md) — 矩形绘制对象
- [QMDrawPixmap](QMDrawPixmap.md) — 位图绘制对象

***