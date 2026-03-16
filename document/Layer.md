# Layer
***

图层类，继承自 `AbstractLayer<QMDrawObject>`。管理绘制对象的图层，提供图层位图合成和绘制对象切换功能。

## 构造函数

### `Layer()`
默认构造函数，创建空图层
***

## 公共方法

### `QPixmap pixmap()`
获取图层合成后的位图，将下方缓存、活动绘制对象、上方缓存按顺序合成  
**Returns** — 合成后的位图
***

### `QSharedPointer<QMDrawObject> setActiveObject(QSharedPointer<QMDrawObject> object) override`
设置活动绘制对象，重绘下方缓存位图  
*object* — 新的活动绘制对象智能指针  
**Returns** — 原活动绘制对象智能指针
***

### `void switchActiveObject(int index) override`
切换到指定索引的绘制对象，重绘上下方缓存位图  
*index* — 目标绘制对象索引
***

### `void finishActiveObject() override`
完成当前活动绘制对象的绘制，将其渲染到下方缓存
***

### `void update() override`
更新图层缓存，重新合成上下方缓存位图  
重新初始化基底，遍历所有绘制对象，将活动对象上方的绘制到 `up_` 缓存，下方的绘制到 `down_` 缓存  
**Note** — 此方法在图层内容变化时调用，用于优化渲染性能
***

## 继承的方法

继承自 [AbstractLayer](AbstractLayer.md)：

### `const QList<QSharedPointer<QMDrawObject>>& items() const`
获取当前管理的所有绘制对象列表

### `bool hide() const`
获取当前隐藏状态

### `void setHide(bool hide)`
设置隐藏状态

### `QMDrawObject* activeObject()`
获取当前活动的绘制对象指针

### `void undo()`
撤销操作

### `void redo()`
重做操作

***

## 相关类

- [AbstractLayer](AbstractLayer.md) — 模板基类
- [QMDrawObject](QMDrawObject.md) — 绘制对象基类
- [LayerManager](LayerManager.md) — 图层管理器

***
