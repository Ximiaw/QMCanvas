# LayerManager
***

图层管理器类，继承自 `AbstractLayer<Layer>`。管理多个图层的合成渲染，提供图层切换、隐藏控制和位图缓存功能。

## 构造函数

### `LayerManager(QObject* parent = nullptr)`
初始化图层管理器，创建 500x500 的透明基底，并添加一个默认图层作为活动图层  
*parent* — 父对象指针
***

### `LayerManager(const QSize& baseSize, QObject* parent = nullptr)`
初始化图层管理器，使用指定尺寸的透明基底  
*baseSize* — 基底尺寸  
*parent* — 父对象指针
***

## 公共方法

### `bool hide(int index)`
获取指定索引图层的隐藏状态  
*index* — 图层索引  
**Returns** — true 表示隐藏，false 表示显示，索引越界返回 true
***

### `void setHide(int index, bool hide)`
设置指定索引图层的隐藏状态  
*index* — 图层索引  
*hide* — true 表示隐藏，false 表示显示
***

### `QSharedPointer<Layer> setActiveObject(QSharedPointer<Layer> object) override`
设置活动图层，并重绘上下方图层的缓存位图  
*object* — 新的活动图层智能指针  
**Returns** — 原活动图层智能指针
***

### `void switchActiveObject(int index) override`
切换到指定索引的图层，并重绘上下方图层的缓存位图  
*index* — 目标图层索引
***

### `void swap(int a, int b)`
交换两个图层的位置  
*a* — 第一个图层索引  
*b* — 第二个图层索引
***

### `QPixmap pixmap()`
获取合成后的位图，将下方缓存、活动图层、上方缓存按顺序合成  
**Returns** — 合成后的位图
***

## 公共槽函数

### `void onHideChanged(int i, bool hide)`
响应图层隐藏状态变化  
*i* — 图层索引  
*hide* — 新的隐藏状态
***

## 继承的方法

继承自 [AbstractLayer](AbstractLayer.md)：

### `const QList<QSharedPointer<Layer>>& items() const`
获取当前管理的所有图层列表

### `bool hide() const`
获取当前隐藏状态（已删除，请使用 hide(int index)）

### `void setHide(bool hide)`
设置隐藏状态（已删除，请使用 setHide(int index, bool hide)）

### `Layer* activeObject()`
获取当前活动的图层指针

### `void finishActiveObject()`
完成当前活动图层的操作

### `void undo()`
撤销操作

### `void redo()`
重做操作

***

## 相关类

- [AbstractLayer](AbstractLayer.md) — 模板基类
- [Layer](Layer.md) — 图层类

***
