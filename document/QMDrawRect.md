# QMDrawRect
***

矩形绘制对象类，继承自 `QMDrawObject`。支持描边和填充两种模式，根据起始点和结束点确定矩形区域。

## 构造函数

### `QMDrawRect(QColor color=Qt::blue, int width=2, bool fill=false)`
初始化矩形绘制对象，设置颜色、线宽和填充模式  
*color* — 矩形边框颜色，默认为蓝色  
*width* — 边框线宽（像素），默认为 2 像素  
*fill* — 是否填充矩形，true 表示填充（使用浅色调），false 表示仅描边
***

## 公共方法

### `void draw(QPainter* painter) override`
执行绘制操作，根据 begin_ 和 end_ 确定的矩形区域进行绘制  
**Note** — 自动计算矩形左上角坐标和宽高，支持反向拖拽（end 在 begin 左上也可正常绘制）  
*painter* — QPainter 绘制对象
***

### `QPen* pen() override`
获取绘制使用的画笔（边框线）  
**Returns** — 画笔指针，包含颜色和宽度设置
***

### `QBrush* brush() override`
获取绘制使用的画刷（填充）  
**Returns** — 画刷指针，若构造时 fill=false 则返回透明画刷（Qt::NoBrush）
***

### `QSharedPointer<QMDrawObject> clone() override`
克隆当前绘制对象，创建具有相同颜色、线宽和填充属性的新实例  
**Returns** — 新的矩形绘制对象智能指针
***

## 继承的方法

继承自 [QMDrawObject](QMDrawObject.md)：

### `void begin(QPoint point)`
开始绘制操作，记录矩形起始角坐标  
*point* — 矩形起始角（左上角或右下角，仅作记录）

### `void end(QPoint point)`
结束绘制操作，记录矩形对角坐标  
*point* — 矩形对角坐标

### `void recordPoint(QPoint point)`
记录绘制过程中的中间点（矩形绘制中不常用，但继承基类行为）  
*point* — 中间点坐标

### `bool isRecord() const`
获取当前是否处于记录状态

### `QPoint begin() const`
获取矩形起始角坐标

### `QPoint end() const`
获取矩形对角坐标

### `QPoint record() const`
获取当前记录点位置

***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `pen_` | `QPen` | 边框画笔属性 |
| `brush_` | `QBrush` | 填充画刷属性 |
| `fill_` | `bool` | 是否填充矩形的标志 |

## 继承的成员

继承自 QMDrawObject：

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `isRecord_` | `bool` | 是否处于记录状态 |
| `begin_` | `QPoint` | 矩形起始角 |
| `end_` | `QPoint` | 矩形对角 |
| `record_` | `QPoint` | 当前记录点 |

***

## 相关类

- [QMDrawObject](QMDrawObject.md) — 绘制对象基类
- [QMDrawPen](QMDrawPen.md) — 手绘线条对象
- [QMDrawPixmap](QMDrawPixmap.md) — 位图绘制对象

***