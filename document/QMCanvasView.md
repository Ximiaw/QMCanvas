# QMCanvasView
***

画布视图容器类，继承自 `QWidget`。整合 View 和 Viewport，提供统一的画布操作接口。支持按住 Ctrl 键临时切换滚轮模式为缩放。

## 属性

| 属性名 | 类型 | 说明 |
|--------|------|------|
| `wheelMode` | `WheelMode` | 滚轮模式，可读可写 |
| `viewport` | `const Viewport*` | 视口指针，只读 |
| `canvasScene` | `QMCanvasScene*` | 场景指针，可读可写，变化时发送信号 |

## 构造函数

### `QMCanvasView(QWidget* parent=nullptr)`
初始化画布视图对象，创建内部 View 和 Viewport 并设置布局  
*parent* — 父窗口指针
***

### `QMCanvasView(QMCanvasScene* scene, QWidget* parent=nullptr)`
初始化画布视图对象，并设置渲染场景  
*scene* — 要关联的场景对象指针  
*parent* — 父窗口指针
***

## 公共方法

### `void setWheelMode(WheelMode mode)`
设置滚轮模式，控制滚轮事件的行为（滚动或缩放）  
*mode* — 滚轮模式，ROLL 表示滚动视图，ZOOM 表示缩放视图
***

### `WheelMode wheelMode() const`
获取当前滚轮模式  
**Returns** — 当前滚轮模式
***

### `void setCanvasScene(QMCanvasScene* scene)`
设置渲染场景，视图将显示该场景中的内容。会调用 scene->init() 建立信号槽连接  
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

## 保护方法

### `void resizeEvent(QResizeEvent* event) override`
重写大小变化事件，发送 sizeChanged 信号通知场景更新  
*event* — 大小变化事件对象
***

### `void keyPressEvent(QKeyEvent* event) override`
重写键盘按下事件，当按下 Ctrl 键时自动切换到 ZOOM 模式  
*event* — 键盘事件对象
***

### `void keyReleaseEvent(QKeyEvent* event) override`
重写键盘释放事件，当释放 Ctrl 键时自动切换回 ROLL 模式  
*event* — 键盘事件对象
***

## 信号

### `void canvasSceneChanged(QMCanvasScene* scene)`
修改场景时发送信号  
*scene* — 新的场景对象指针
***

### `void sizeChanged(QSize size)`
视图大小变化时发送信号  
*size* — 新的视图大小
***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `view_` | `View` | 内部滚动视图对象 |
| `viewport_` | `Viewport` | 内部视口对象 |
| `scenePointer_` | `QSharedPointer<QMCanvasScene>` | 场景智能指针 |

***
