// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "Layer.h"
#include <QLabel>


QPixmap Layer::pixmap(){
    QPixmap pixmap = QPixmap(base_);
    QPainter painter(&pixmap);

    painter.drawPixmap(down_.rect(),down_);
    if (!activeItem_.isNull())
        activeItem_.get()->draw(&painter);
    painter.drawPixmap(up_.rect(),up_);
    return pixmap;
}

QSharedPointer<QMDrawObject> Layer::setActiveObject(QSharedPointer<QMDrawObject> object){
    QPainter pd(&down_);
    auto obj = AbstractLayer::setActiveObject(object);
    for (auto draw = items().begin();draw!=items().end();draw++){
        if (draw->isNull() || draw->get() == object.get()) continue;
        draw->get()->draw(&pd);
    }
    return obj;
}

void Layer::switchActiveObject(int index){
    AbstractLayer::switchActiveObject(index);
    update();
}

void Layer::finishActiveObject(){
    QPainter painter(&down_);
    activeItem_.get()->draw(&painter);
    auto obj = activeItem_.get();
    AbstractLayer::finishActiveObject();
    setActiveObject(QSharedPointer<QMDrawObject>(obj->clone()));
}

void Layer::update(){
    setBase(base_.size());
    QPainter pd(&down_);
    QPainter pu(&up_);
    bool down = true;
    for (auto draw = items().begin();draw!=items().end();draw++){
        if (draw->isNull()) continue;
        if (draw->get()==activeItem_.get()) down = false;
        if (down)
            draw->get()->draw(&pd);
        else
            draw->get()->draw(&pu);
    }
}

void Layer::clear(){
    auto obj = activeItem_->clone();
    items_.clear();
    setActiveObject(QSharedPointer<QMDrawObject>(obj));
    update();
}
