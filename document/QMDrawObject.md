# QMDrawObject
***

### `QMDrawObject(QObject* parent)`
初始化绘制对象，parent 为父对象指针
***

### `~QMDrawObject()`
默认析构函数
***

### `virtual void draw(QPainter* painter) = 0`
纯虚函数，执行绘制操作。子类必须实现此方法  
*painter* — QPainter 绘制对象，用于执行具体的绘制命令
***

### `virtual void begin(QPoint point) = 0`
纯虚函数，开始绘制操作。子类必须实现此方法  
*point* — 绘制起始点坐标
***

### `virtual void end(QPoint point) = 0`
纯虚函数，结束绘制操作。子类必须实现此方法  
*point* — 绘制结束点坐标
***

### `virtual void recordPoint(QPoint point) = 0`
纯虚函数，记录绘制过程中的中间点。子类必须实现此方法  
**注意**：必须在 begin 调用之后才能记录点，begin 前禁止记录  
*point* — 需要记录的中间点坐标
***
