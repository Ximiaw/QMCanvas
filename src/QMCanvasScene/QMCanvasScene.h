// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_QMCANVASSCENE_H
#define MAIN_QMCANVASSCENE_H

#include <QObject>
#include <QList>
#include <QRectF>
#include <QImage>
#include <QPixmap>

#include "LayerManager.h"
#include "Location.h"
#include "QMDrawObject.h"
#include "QMDrawPixmap.h"

class QMCanvasView;
class View;
class Viewport;

class QMCanvasScene : public QObject {
    Q_OBJECT
private:
    Location location_;
    LayerManager layerManager_;

    bool mouseMove_=false;
public:
    QMCanvasScene(QPixmap pixmap=QPixmap(500,500),QObject* parent=nullptr);
    ~QMCanvasScene() override = default;

    bool isMove() const;
    void beginMove();
    void endMove();

    qreal maxRatio() const;
    void setMaxRatio(qreal max);

    qreal minRatio() const;
    void setMinRatio(qreal min);

    const QPixmap pixmap();

    //这几个函数和viewport交互
    QPixmap getViewportPixmap();
    QRect getViewportRect();
    void draw(QPainter* painter);

    void init(QMCanvasView* canvasView,View* view,Viewport* viewport);

    qreal factor() const;
    void setFactor(qreal factor);

    qreal extraViewportMargin() const;
    void setExtraViewportMargin(qreal rate);

    qreal ratio() const;
    void setRatio(qreal ratio);

    QMDrawObject* activeDrawObject();
    void setActiveDrawObject(QMDrawObject* object);//会delete旧的活跃绘图对象
    void switchDrawObject(int i);
    void finishActiveDrawObject();

    Layer* activeLayer();
    void setActiveLayer(Layer* layer);//会delete旧的活跃绘图对象
    void switchLayer(int i);
    void finishActiveLayer();

    void swapLayer(int a,int b);
    bool isLayerHide(int index);
    void setLayerHide(int index,bool hide);
private:
    void inform();//通知所有信号发送
public slots:
    void onViewportChanged(QRectF rect);//需要在这里处理外扩
    void onScaleBy(bool magnify,QPoint point);
    void onSizeChanged();
    void onMouseMove(QPoint point);
    void onMouseRelease(QPoint point);
    void onMousePress(QPoint point);
    void onHScrollBarChanged(int value);
    void onVScrollBarChanged(int value);
    void onCtrlAndZ();
    void onCtrlAndY();
signals:
    void viewportRectChanged();//onViewportChanged和onScaleBy以及view的滚动条滚动触发
    void viewportPixmapChanged();
    void viewPropertyChanged(QPoint point,QSize size);//通知view更新滚动条和缩放
};


#endif //MAIN_QMCANVASSCENE_H