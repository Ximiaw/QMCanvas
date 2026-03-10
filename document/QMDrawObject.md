# QMDrawObject
***

绘制对象抽象基类，继承自 `QObject`。所有自定义绘制对象必须继承此类并实现纯虚函数。

## 构造函数

### `QMDrawObject(QObject* parent)`
初始化绘制对象，parent 为父对象指针
***

### `~QMDrawObject()`
默认析构函数
***

## 纯虚方法（子类必须实现）

### `virtual void draw(QPainter* painter) = 0`
执行绘制操作  
*painter* — QPainter 绘制对象，用于执行具体的绘制命令
***

### `virtual void begin(QPoint point) = 0`
开始绘制操作  
*point* — 绘制起始点坐标
***

### `virtual void end(QPoint point) = 0`
结束绘制操作  
*point* — 绘制结束点坐标
***

### `virtual void recordPoint(QPoint point) = 0`
记录绘制过程中的中间点  
**注意**：必须在 begin 调用之后才能记录点，begin 前禁止记录  
*point* — 需要记录的中间点坐标
***
