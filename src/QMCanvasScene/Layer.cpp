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
    down_ = QPixmap(base_);
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
    if (activeItem_.isNull()) return;
    QPainter painter(&down_);
    activeItem_->draw(&painter);
    auto ptr = activeItem_->clone();
    AbstractLayer::finishActiveObject();
    painter.end();
    setActiveObject(QSharedPointer<QMDrawObject>(ptr));
}

void Layer::undo(){
    QSharedPointer<QMDrawObject> ptr=nullptr;
    if (!activeItem_.isNull())
        ptr = activeItem_->clone();
    AbstractLayer::undo();
    setActiveObject(ptr);
    update();
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
    QSharedPointer<QMDrawObject> ptr=nullptr;
    if (!activeItem_.isNull())
        ptr = activeItem_->clone();
    items_.clear();
    setActiveObject(ptr);
    update();
}
