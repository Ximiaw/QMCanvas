# Location
***

位置管理类，负责处理坐标变换、视口计算和缩放逻辑。

## 构造函数

### `Location(QRectF baseRect)`
初始化位置管理器，设置基础区域
*baseRect* — 基础区域矩形，所有坐标变换基于此区域
***

### `~Location()`
默认析构函数
***

## 公共方法

### `QRectF baseRect()`
获取基础区域
**Returns** — 基础区域矩形
***

### `void setBaseRect(QRectF baseRect)`
设置基础区域
*baseRect* — 新的基础区域矩形
***

### `QRectF viewportRect()`
获取视口矩形（未缩放未外扩）
**Returns** — 原始视口矩形
***

### `void setViewportRect(QRectF rect)`
设置视口矩形，根据当前缩放倍率计算未缩放的视口坐标
*rect* — 新的视口矩形（已缩放坐标）
***

### `void setViewportRect(QSize size)`
设置视口矩形大小，保持当前位置不变
*size* — 新的视口大小
***

### `QRectF viewRectR()`
获取经过缩放后的视图矩形（整个基础区域的缩放后大小）
**Returns** — 缩放后的视图矩形
***

### `QRectF viewportRectRM()`
获取经过缩放和外扩后的视口矩形，考虑鼠标位置偏移和边界限制
**Returns** — 缩放外扩后的视口矩形
***

### `void setMousePoint(QPoint point)`
设置鼠标位置，用于计算视口偏移（缩放时以鼠标为中心）
*point* — 鼠标位置（未缩放坐标）
***

### `qreal maxRatio() const`
获取最大缩放倍率
**Returns** — 最大缩放倍率，默认为 20.0
***

### `void setMaxRatio(qreal max)`
设置最大缩放倍率
*max* — 最大缩放倍率
***

### `qreal minRatio() const`
获取最小缩放倍率
**Returns** — 最小缩放倍率，默认为 0.01
***

### `void setMinRatio(qreal min)`
设置最小缩放倍率
*min* — 最小缩放倍率
***

### `qreal factor() const`
获取每次缩放的倍率
**Returns** — 缩放倍率，默认为 1.0
***

### `void setFactor(qreal factor)`
设置每次缩放的倍率
*factor* — 缩放倍率
***

### `qreal extraViewportMargin() const`
获取当前视口外扩倍率
**Returns** — 外扩倍率，1 表示无外扩，默认为 1.0
***

### `void setExtraViewportMargin(qreal rate)`
设置视口外扩倍率
*rate* — 外扩倍率
***

### `qreal ratio() const`
获取当前图像显示倍率
**Returns** — 显示倍率，默认为 1.0
***

### `void setRatio(qreal ratio)`
设置当前图像显示倍率
*ratio* — 显示倍率
***

## 私有成员

| 成员名 | 类型 | 说明 |
|--------|------|------|
| `baseRect_` | `QRectF` | 基础区域 |
| `viewportRect_` | `QRectF` | 原始视口矩形（未缩放） |
| `considerMouse_` | `bool` | 是否根据鼠标计算偏移 |
| `mousePoint_` | `QPoint` | 鼠标位置 |
| `factor_` | `qreal` | 每次缩放倍率 |
| `ratio_` | `qreal` | 当前显示倍率 |
| `marginRate_` | `qreal` | 外扩倍率 |
| `maxRatio_` | `qreal` | 最大缩放倍率 |
| `minRatio_` | `qreal` | 最小缩放倍率 |

***
