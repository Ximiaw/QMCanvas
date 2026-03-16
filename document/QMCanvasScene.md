# QMCanvasScene
***

场景管理器类，继承自 `QObject`。负责管理绘制对象列表、处理坐标变换、维护视口状态和位图缓存。

## 构造函数

### `QMCanvasScene(QPixmap pixmap=QPixmap(500,500), QObject* parent=nullptr)`
初始化场景对象  
*pixmap* — 初始位图，默认为 500x500 的空白位图  
*parent* — 父对象指针
***

### `~QMCanvasScene()`
默认析构函数
***

## 公共方法

### `bool isMove() const`
获取当前是否处于鼠标移动状态  
**Returns** — true 表示鼠标正在移动，false 表示未移动
***

### `void beginMove()`
开始鼠标移动操作，设置 mouseMove_ 标志为 true
***

### `void endMove()`
结束鼠标移动操作，设置 mouseMove_ 标志为 false
***

### `qreal maxRatio() const`
获取最大缩放倍率  
**Returns** — 最大缩放倍率，默认为 20.0
***

### `void setMaxRatio(qreal max)`
设置最大缩放倍率  
*max* — 最大缩放倍率
***

### `qreal minRatio() const`
获取最小缩放倍率  
**Returns** — 最小缩放倍率，默认为 0.01
***

### `void setMinRatio(qreal min)`
设置最小缩放倍率  
*min* — 最小缩放倍率
***

### `const QPixmap pixmap()`
获取当前场景的底图  
**Returns** — 当前位图
***

### `QPixmap getViewportPixmap()`
获取当前视口区域的位图（经过缩放和外扩处理）  
**Returns** — 视口区域的位图
***

### `QRect getViewportRect()`
获取当前视口矩形（经过缩放和外扩处理）  
**Returns** — 视口矩形
***

### `void draw(QPainter* painter)`
执行绘制操作，将所有绘制对象绘制到 painter 上  
*painter* — QPainter 绘制对象
***

### `void init(QMCanvasView* canvasView, View* view, Viewport* viewport)`
初始化场景的信号槽连接，建立与 View、Viewport、QMCanvasView 的通信  
*canvasView* — 画布视图指针  
*view* — 滚动视图指针  
*viewport* — 视口指针
***

### `qreal factor() const`
获取每次缩放的倍率  
**Returns** — 缩放倍率，默认为 1.0
***

### `void setFactor(qreal factor)`
设置每次缩放的倍率  
*factor* — 缩放倍率，如 1.1 表示每次放大 10%
***

### `qreal extraViewportMargin() const`
获取当前视口外扩边距比例  
**Returns** — 外扩比例（1 表示无额外边距，1.2 表示四周各外扩 20%）
***

### `void setExtraViewportMargin(qreal rate)`
设置视口外扩边距比例，用于在可视区域外预渲染内容  
*rate* — 外扩比例，基于当前视口尺寸
***

### `qreal ratio() const`
获取当前图像显示倍率  
**Returns** — 当前倍率，如 1.2 表示原图的 1.2 倍
***

### `void setRatio(qreal ratio)`
设置当前图像显示倍率，会触发 inform() 更新视图  
*ratio* — 显示倍率
***

## 绘制对象管理

### `QMDrawObject* activeDrawObject()`
获取当前活动的绘图对象  
**Returns** — 活动绘图对象指针，若无则返回 nullptr
***

### `void setActiveDrawObject(QMDrawObject* object)`
设置当前活动的绘图对象  
*object* — 新的活动绘图对象指针
***

### `void switchDrawObject(int i)`
切换到指定索引的绘图对象  
*i* — 目标绘图对象索引
***

### `void finishActiveDrawObject()`
完成当前活动绘图对象的绘制，将其添加到绘制对象列表中
***

## 图层管理

### `Layer* activeLayer()`
获取当前活动的图层  
**Returns** — 活动图层指针，若无则返回 nullptr
***

### `void setActiveLayer(Layer* layer)`
设置当前活动的图层  
*layer* — 新的活动图层指针
***

### `void switchLayer(int i)`
切换到指定索引的图层  
*i* — 目标图层索引
***

### `void finishActiveLayer()`
完成当前活动图层的操作
***

### `void swapLayer(int a, int b)`
交换两个图层的位置  
*a* — 第一个图层索引  
*b* — 第二个图层索引
***

### `bool isLayerHide(int index)`
获取指定索引图层的隐藏状态  
*index* — 图层索引  
**Returns** — true 表示隐藏，false 表示显示
***

### `void setLayerHide(int index, bool hide)`
设置指定索引图层的隐藏状态  
*index* — 图层索引  
*hide* — true 表示隐藏，false 表示显示
***

## 私有方法

### `void inform()`
通知所有信号发送，触发 viewportRectChanged、viewportPixmapChanged 和 viewPropertyChanged 信号
***

## 公共槽函数

### `void onScaleBy(bool magnify, QPoint point)`
相对缩放时调用，根据 magnify 执行放大或缩小操作  
*magnify* — 缩放判断，true 表示放大，false 表示缩小  
*point* — 缩放中心点
***

### `void onSizeChanged()`
视图大小变化时调用，触发 inform() 更新视图属性
***

### `void onMouseMove(QPoint point)`
鼠标移动时调用，转发给活动绘图对象的 recordPoint()  
*point* — 鼠标当前位置
***

### `void onMouseRelease(QPoint point)`
鼠标释放时调用，结束当前绘制操作并将活动对象添加到列表  
*point* — 鼠标释放位置
***

### `void onMousePress(QPoint point)`
鼠标按下时调用，开始当前绘制操作  
*point* — 鼠标按下位置
***

### `void onHScrollBarChanged(int value)`
水平滚动条值变化时调用，更新视口矩形 X 坐标  
*value* — 滚动条当前值
***

### `void onVScrollBarChanged(int value)`
垂直滚动条值变化时调用，更新视口矩形 Y 坐标  
*value* — 滚动条当前值
***

### `void onCtrlAndZ()`
响应 Ctrl+Z 快捷键，执行撤销操作
***

### `void onCtrlAndY()`
响应 Ctrl+Y 快捷键，执行重做操作
***

## 信号

### `void viewportRectChanged()`
视口矩形变化时发送信号（由 onScaleBy 和滚动条变化触发）
***

### `void viewportPixmapChanged()`
视口位图变化时发送信号
***

### `void viewPropertyChanged(QPoint point, QSize size)`
通知 View 更新滚动条位置和视图大小  
*point* — 新的滚动位置  
*size* — 新的视图大小
***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `location_` | `Location` | 位置管理对象 |
| `layerManager_` | `LayerManager` | 图层管理器 |
| `mouseMove_` | `bool` | 鼠标移动状态标志 |

***
