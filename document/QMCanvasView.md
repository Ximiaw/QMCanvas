# WheelMode 滚轮模式
- *ROLL* 滚动 — 滚轮旋转时平移视图
- *ZOOM* 缩放 — 滚轮旋转时缩放视图
***

# QMCanvasView
***
### `void setWheelMode(WheelMode mode)`
设置滚轮模式
***
### `WheelMode wheelMode() const`
获取当前模式
***
### `void setCanvasScene(QMCanvasScene* scene)`
设置渲染场景
***
### `QMCanvasScene* canvasScene() const`
获取渲染场景
***
### `const Viewport* viewport() const`
获取视窗指针