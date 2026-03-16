// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_LAYER_H
#define MAIN_LAYER_H

#include <QPixmap>
#include <QSharedPointer>
#include <QPainter>
#include "AbstractLayer.h"
#include "QMDrawObject.h"

class Layer : public AbstractLayer<QMDrawObject>{
public:
    QPixmap pixmap();

    QSharedPointer<QMDrawObject> setActiveObject(QSharedPointer<QMDrawObject> object) override;
    void switchActiveObject(int index) override;
    void finishActiveObject() override;
    void update() override;
};

#endif //MAIN_LAYER_H