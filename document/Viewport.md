# Viewport
***

视口渲染部件类，继承自 `QWidget`，负责实际的绘制渲染和鼠标事件转发。

## 构造函数

### `Viewport(QWidget* parent=nullptr)`
初始化视口对象，parent 为父窗口指针。自动禁用背景填充、启用鼠标追踪并安装事件过滤器
***

### `~Viewport()`
默认析构函数
***

## 保护方法

### `void paintEvent(QPaintEvent *event) override`
重写绘制事件，委托关联的 QMCanvasScene 进行绘制  
*event* — 绘制事件对象
***

### `void mouseMoveEvent(QMouseEvent* event) override`
重写鼠标移动事件，当左键按下且场景处于移动状态时发送鼠标移动信号  
*event* — 鼠标事件对象
***

### `void mouseReleaseEvent(QMouseEvent* event) override`
重写鼠标释放事件，当左键释放时发送鼠标释放信号并结束移动状态  
*event* — 鼠标事件对象
***

### `void mousePressEvent(QMouseEvent* event) override`
重写鼠标按下事件，当左键按下时发送鼠标按下信号并开始移动状态  
*event* — 鼠标事件对象
***

### `bool eventFilter(QObject* watched, QEvent* event) override`
重写事件过滤器，处理键盘快捷键（Ctrl+Z 撤销、Ctrl+Y 重做、Ctrl+S 保存）  
*watched* — 被监视的对象  
*event* — 事件对象  
**Returns** — 事件已处理返回 true，否则返回 false
***

## 公共槽函数

### `void onSceneChanged(QMCanvasScene* scene)`
场景对象变化时调用，更新场景指针并同步视口位置  
*scene* — 新的场景对象指针
***

### `void onRectChanged()`
场景视口矩形变化时调用，同步更新本视口的几何位置和大小，并触发重绘
***

### `void onPixmapChanged()`
场景位图变化时调用，触发重绘更新显示
***

## 信号

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

### `void ctrlAndZ()`
Ctrl+Z 快捷键触发时发送信号，用于撤销操作
***

### `void ctrlAndY()`
Ctrl+Y 快捷键触发时发送信号，用于重做操作
***

### `void save()`
Ctrl+S 快捷键触发时发送信号，用于保存当前状态或导出图片（需自行实现）
***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `scenePointer_` | `QPointer<QMCanvasScene>` | 关联的场景指针 |

***
