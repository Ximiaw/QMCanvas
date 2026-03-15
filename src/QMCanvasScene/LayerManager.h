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
public:
    LayerManager(QObject* parent=nullptr);
    LayerManager(const QSize& baseSize,QObject* parent=nullptr);

    bool hide(int index);
    bool hide()=delete;

    QSharedPointer<Layer> setActiveObject(QSharedPointer<Layer> object) override;
    void switchActiveObject(int index) override;

    void swap(int a,int b);

    QPixmap pixmap();

public slots:
    void onHideChanged(int i,bool hide);//第几个图层需要隐藏|显示
};

#endif //MAIN_QMLAYERMANAGER_H
