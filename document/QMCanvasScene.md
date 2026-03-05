# QMCanvasScene
***

### `QMCanvasScene(QObject* parent)`
初始化场景对象，parent 为父对象指针
***

### `const QList&lt;QMDrawObject*&gt; graphicList()`
获取当前场景中的所有绘制对象列表
***

### `void addGraphic(QMDrawObject* graphic)`
向场景中添加绘制对象，scene 将接管该对象的生命周期管理
***

### `void removeGraphic(QMDrawObject* graphic)`
从场景中移除指定的绘制对象，**不会** delete 该对象
***

### `void deleteGraphic(QMDrawObject* graphic)`
从场景中移除并 delete 指定的绘制对象
***

### `void setExtraViewportMargin(qreal rate)`
设置视口外扩边距比例，用于在可视区域外预渲染内容。  
*rate* — 外扩比例，基于当前视口尺寸（1 表示无额外边距，1.2 表示四周各外扩 20%）
***

### `qreal extraViewportMargin() const`
获取当前视口外扩边距比例  
**Returns** — 外扩比例（1 表示无额外边距，1.2 表示四周各外扩 20%）
***

## slots
***

### `void onViewportChanged(QRectF rect)`
视口几何变化时调用，内部更新可见性判断区域  
*rect* — 新的视口矩形（场景坐标系）
***

### `void onScaleBy(bool magnify)`
相对缩放时调用，用于优化重绘策略或 LOD 切换  
*magnify* — 缩放判断，true放大，false缩小
***