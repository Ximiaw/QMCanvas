// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_QMCANVASSCENE_H
#define MAIN_QMCANVASSCENE_H

#include <QObject>
#include <QList>
#include <QRectF>
#include <QImage>
#include <QPixmap>

#include "QMDrawObject.h"
#include "LayerManager.h"

class QMCanvasView;
class View;
class Viewport;

class QMCanvasScene : public QObject {
    Q_OBJECT
private:
    QList<QMDrawObject*> drawObject_;
    QMDrawObject* activeDrawObject_=nullptr;//当前活动的绘图对象，按下鼠标左键开始记录点，释放结束并放入列表
    QPixmap pixmap_;
    QRectF viewportRect_;//未缩放未外扩的viewport区域
    QPoint mousePoint_;//鼠标指向的位置，是未缩放未外扩的
    qreal factor_=1.0;//每次缩放倍率，和viewportRate相乘
    qreal ratio_=1.0;//当前图像倍率，如1.2为原图的1.2倍
    qreal marginRate_=1.0;//当前外扩倍率，1为默认值
    qreal maxRatio_=20.0;//最大缩放倍率
    qreal minRatio_=0.01;//最小缩放倍率
    bool mouseMove_=false;
public:
    QMCanvasScene(QPixmap pixmap=QPixmap(500,500),QObject* parent=nullptr);
    ~QMCanvasScene() override = default;

    const QList<QMDrawObject*> graphicList() const;
    void addGraphic(QMDrawObject* graphic);//添加到对象树
    bool deleteGraphic(QMDrawObject* graphic);

    bool isMove() const;
    void beginMove();
    void endMove();

    qreal maxRatio() const;
    void setMaxRatio(qreal max);

    qreal minRatio() const;
    void setMinRatio(qreal min);

    void setPixmap(QPixmap& pixmap);
    const QPixmap& pixmap() const;

    //这几个函数和viewport交互
    QPixmap getViewportPixmap();
    QRect getViewportRect();
    void updatePixmap(QPainter* painter);
    void draw(QPainter* painter);

    void init(QMCanvasView* canvasView,View* view,Viewport* viewport);

    qreal factor() const;
    void setFactor(qreal factor);

    qreal extraViewportMargin() const;
    void setExtraViewportMargin(qreal rate);

    qreal ratio() const;
    void setRatio(qreal ratio);

    QMDrawObject* activeDrawObject() const;
    void setActiveDrawObject(QMDrawObject* object);//会delete旧的活跃绘图对象
    void finishActiveDrawObject();

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