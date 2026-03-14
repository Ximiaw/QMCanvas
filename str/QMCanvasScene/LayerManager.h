// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_QMLAYERMANAGER_H
#define MAIN_QMLAYERMANAGER_H

#include <QList>
#include <QPainter>
#include <QPointer>
#include <QPixmap>
#include <QColor>
#include <QSize>
#include "Layer.h"
#include "AbstractLayer.h"

class LayerManager : public AbstractLayer<Layer>{
    Q_OBJECT
private:
    QPixmap base_;//一个透明基底，先渲染到这个上面，而后才copy需要渲染的地方

public:
    LayerManager(QObject* parent=nullptr);
    LayerManager(const QSize& baseSize,QObject* parent=nullptr);

    bool hide(int index);
    bool hide()=delete;

    QPixmap& base();//获取基底的引用，以便copy
    void setBase(const QSize& baseSize);//必须先设置这个

    QPixmap pixmap();

public slots:
    void onHideChanged(int i,bool hide);//第几个图层需要隐藏|显示
};

#endif //MAIN_QMLAYERMANAGER_H
