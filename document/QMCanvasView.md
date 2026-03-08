# QMCanvasView
***

### `QMCanvasView(QWidget* parent=nullptr)`
初始化画布视图对象，parent 为父窗口指针
***

### `QMCanvasView(QMCanvasScene* scene, QWidget* parent=nullptr)`
初始化画布视图对象，并设置渲染场景  
*scene* — 要关联的场景对象指针  
*parent* — 父窗口指针
***

### `void setWheelMode(WheelMode mode)`
设置滚轮模式，控制滚轮事件的行为（滚动或缩放）  
*mode* — 滚轮模式，ROLL 表示滚动视图，ZOOM 表示缩放视图
***

### `WheelMode wheelMode() const`
获取当前滚轮模式  
**Returns** — 当前滚轮模式
***

### `void setCanvasScene(QMCanvasScene* scene)`
设置渲染场景，视图将显示该场景中的内容  
*scene* — 要关联的场景对象指针
***

### `QMCanvasScene* canvasScene() const`
获取当前关联的渲染场景  
**Returns** — 场景对象指针，若未设置则返回 nullptr
***

### `const Viewport* viewport() const`
获取视口对象指针  
**Returns** — 视口对象指针
***

## signals
***

### `void canvasSceneChanged(QMCanvasScene* scene)`
修改场景时发送信号  
*scene* — 新的场景对象指针
***
