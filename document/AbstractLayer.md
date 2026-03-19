# AbstractLayer
***

图层管理模板基类，继承自 `QObject`。提供对模板类型对象的列表管理、撤销/重做功能和活动对象切换功能。

## 模板参数

| 参数 | 说明 |
|------|------|
| `T` | 管理的对象类型，需支持 QSharedPointer 包装 |

## 构造函数

### `AbstractLayer(QObject* parent = nullptr)`
初始化抽象图层对象，设置 hide_ 为 false，activeItem_ 为空
***

### `~AbstractLayer()`
默认析构函数
***

## 公共方法

### `const QList<QSharedPointer<T>>& items() const`
获取当前管理的所有对象列表  
**Returns** — 对象智能指针列表的常量引用
***

### `QPixmap base()`
获取当前基底位图  
**Returns** — 基底位图副本
***

### `void setBase(const QSize& baseSize)`
设置基底尺寸，创建透明背景位图，并重置上下方缓存位图  
*baseSize* — 基底尺寸（宽度和高度）
***


### `bool hide() const`
获取当前隐藏状态  
**Returns** — true 表示隐藏，false 表示显示
***

### `void setHide(bool hide)`
设置隐藏状态  
*hide* — true 表示隐藏，false 表示显示
***

### `T* activeObject()`
获取当前活动的对象指针  
**Returns** — 活动对象指针，若无则返回 nullptr
***

### `QSharedPointer<T> setActiveObject(QSharedPointer<T> object)`
设置活动对象，将原活动对象从列表中移除（若存在），并将新对象添加到列表末尾  
*object* — 新的活动对象智能指针  
**Returns** — 原活动对象智能指针，若原对象不存在则返回空指针
***

### `void switchActiveObject(int index)`
切换到指定索引的对象作为活动对象  
*index* — 目标对象在列表中的索引
***

### `void finishActiveObject()`
完成当前活动对象的操作，清空活动对象指针和撤销栈
***

### `void removeActiveObject(int index)`
移除指定索引的活动对象  
*index* — 要移除的对象索引  
**Note** — 如果移除的是当前活动对象，会自动清空活动对象指针
***

### `void undo()`
撤销操作，将列表最后一个对象移到撤销栈
***

### `void redo()`
重做操作，将撤销栈最后一个对象移回列表
***

### `virtual void update() = 0`
纯虚函数，更新图层缓存和合成  
子类必须实现此方法以重绘上下方缓存位图
***

## 保护成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `items_` | `QList<QSharedPointer<T>>` | 管理的对象列表 |
| `undoStack_` | `QList<QSharedPointer<T>>` | 撤销栈 |
| `activeItem_` | `QPointer<T>` | 当前活动对象（使用 QPointer 自动置空） |
| `base_` | `QPixmap` | 基底位图，图层合成的底层画布 |
| `up_` | `QPixmap` | 活动图层上方的图层合并成的缓存图，减少每帧计算数量 |
| `down_` | `QPixmap` | 活动图层下方的图层合并成的缓存图，减少每帧计算数量 |
| `hide_` | `bool` | 隐藏状态标志 |

***
