# QMCanvasScene
***

### `QMCanvasScene(QPixmap pixmap=QPixmap(200,200), QObject* parent=nullptr)`
初始化场景对象  
*pixmap* — 初始位图，默认为 200x200 的空白位图  
*parent* — 父对象指针
***

### `const QList<QMDrawObject*> graphicList() const`
获取当前场景中的所有绘制对象列表  
**Returns** — 绘制对象指针列表
***

### `void addGraphic(QMDrawObject* graphic)`
向场景中添加绘制对象，scene 将接管该对象的生命周期管理  
*graphic* — 要添加的绘制对象指针
***

### `bool deleteGraphic(QMDrawObject* graphic)`
从场景中移除并删除指定的绘制对象  
*graphic* — 要删除的绘制对象指针  
**Returns** — 删除成功返回 true，对象不存在返回 false
***

### `void setPixmap(QPixmap& pixmap)`
设置场景的底图  
*pixmap* — 新的位图
***

### `const QPixmap& pixmap() const`
获取当前场景的底图  
**Returns** — 当前位图的常量引用
***

### `QPixmap getViewportPixmap()`
获取当前视口区域的位图（经过缩放和外扩处理）  
**Returns** — 视口区域的位图
***

### `QRect getViewportRect()`
获取当前视口矩形（经过缩放和外扩处理）  
**Returns** — 视口矩形
***

### `void updatePixmap(QPainter* painter)`
更新位图缓存，在需要重新渲染时调用  
*painter* — QPainter 绘制对象
***

### `void draw(QPainter* painter)`
执行绘制操作，将所有绘制对象绘制到 painter 上  
*painter* — QPainter 绘制对象
***

### `qreal factor() const`
获取每次缩放的倍率  
**Returns** — 缩放倍率
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
设置当前图像显示倍率  
*ratio* — 显示倍率
***

### `QMDrawObject* activeDrawObject() const`
获取当前活动的绘图对象  
**Returns** — 活动绘图对象指针，若无则返回 nullptr
***

### `void setActiveDrawObject(QMDrawObject* object)`
设置当前活动的绘图对象，会 delete 旧的活动绘图对象  
*object* — 新的活动绘图对象指针
***

## public slots
***

### `void onViewportChanged(QRectF rect)`
视口几何变化时调用，内部更新可见性判断区域并处理外扩  
*rect* — 新的视口矩形（场景坐标系）
***

### `void onScaleBy(bool magnify, QPoint point)`
相对缩放时调用，用于执行缩放操作  
*magnify* — 缩放判断，true 表示放大，false 表示缩小  
*point* — 缩放中心点
***

### `void onMouseMove(QPoint point)`
鼠标移动时调用，转发给活动绘图对象  
*point* — 鼠标当前位置
***

### `void onMouseRelease(QPoint point)`
鼠标释放时调用，结束当前绘制操作  
*point* — 鼠标释放位置
***

### `void onMousePress(QPoint point)`
鼠标按下时调用，开始当前绘制操作  
*point* — 鼠标按下位置
***

## signals
***

### `void viewportRectChanged()`
视口矩形变化时发送信号
***

### `void viewportPixmapChanged()`
视口位图变化时发送信号
***

### `void viewPropertyChanged(QPoint point, QSize size)`
通知 View 更新滚动条位置和视图大小  
*point* — 新的滚动位置  
*size* — 新的视图大小
***
