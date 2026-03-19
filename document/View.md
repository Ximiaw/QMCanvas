# View
***

滚动视图容器类，继承自 `QScrollArea`，处理滚轮事件和视口变化通知。

## 构造函数

### `View(QWidget* parent=nullptr)`
初始化视图对象，parent 为父窗口指针
***

### `~View()`
析构函数
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

## 保护方法

### `void wheelEvent(QWheelEvent *event) override`
重写滚轮事件处理，根据当前 wheelMode 决定执行平移或缩放操作  
*event* — 滚轮事件对象
***

## 私有方法

### `void factor(QWheelEvent *event)`
处理视图缩放，根据滚轮角度计算缩放方向并发送 scaleFactorChanged 信号  
*event* — 滚轮事件对象，用于获取滚轮旋转角度
***

## 公共槽函数

### `void onPropertyChanged(QPoint point, QSize size)`
响应属性变化，更新滚动条位置和视图部件大小  
*point* — 新的滚动位置  
*size* — 新的视图大小
***

## 信号

### `void scaleFactorChanged(bool magnify, QPoint point)`
缩放因子变化时发送信号  
*magnify* — 缩放方向，true 表示放大，false 表示缩小  
*point* — 缩放中心点（鼠标位置）
***

### `void viewportChanged(QRectF rect)`
视口矩形变化时发送信号  
*rect* — 新的视口矩形
***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `wheelMode_` | `WheelMode` | 当前滚轮模式，默认为 ROLL |

***
