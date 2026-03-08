# Viewport
***

### `Viewport(QWidget* parent=nullptr)`
初始化视口对象，parent 为父窗口指针。自动禁用背景填充并启用鼠标追踪
***

### `~Viewport()`
默认析构函数
***

## protected
***

### `void paintEvent(QPaintEvent *event) override`
重写绘制事件，委托关联的 QMCanvasScene 进行绘制  
*event* — 绘制事件对象
***

### `void mouseMoveEvent(QMouseEvent* event) override`
重写鼠标移动事件，当左键按下时发送鼠标移动信号  
*event* — 鼠标事件对象
***

### `void mouseReleaseEvent(QMouseEvent* event) override`
重写鼠标释放事件，当左键释放时发送鼠标释放信号  
*event* — 鼠标事件对象
***

### `void mousePressEvent(QMouseEvent* event) override`
重写鼠标按下事件，当左键按下时发送鼠标按下信号  
*event* — 鼠标事件对象
***

## public slots
***

### `void onSceneChanged(QMCanvasScene* scene)`
场景对象变化时调用，更新场景指针并建立信号槽连接  
*scene* — 新的场景对象指针
***

### `void onRectChanged()`
场景视口矩形变化时调用，同步更新本视口的几何位置和大小
***

### `void onPixmapChanged()`
场景位图变化时调用，标记需要重新更新位图缓存
***

## signals
***

### `void mouseMove(QPoint point)`
鼠标移动时发送信号（左键按下状态）  
*point* — 鼠标当前位置（相对于父窗口的坐标）
***

### `void mouseRelease(QPoint point)`
鼠标释放时发送信号（左键释放）  
*point* — 鼠标释放位置（相对于父窗口的坐标）
***

### `void mousePress(QPoint point)`
鼠标按下时发送信号（左键按下）  
*point* — 鼠标按下位置（相对于父窗口的坐标）
***
