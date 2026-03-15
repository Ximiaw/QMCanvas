// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "LayerManager.h"

LayerManager::LayerManager(QObject* parent):AbstractLayer(parent){
    setBase(QSize(500,500));
    LayerManager::setActiveObject(QSharedPointer<Layer>(new Layer));
}

LayerManager::LayerManager(const QSize& baseSize,QObject* parent):AbstractLayer(parent){
    setBase(baseSize);
    LayerManager::setActiveObject(QSharedPointer<Layer>(new Layer));
}

bool LayerManager::hide(int index){
    if (items_.size()<=index||index<0) return true;
    return items_.at(index).get()->hide();
}

QSharedPointer<Layer> LayerManager::setActiveObject(QSharedPointer<Layer> object){
    auto obj = AbstractLayer::setActiveObject(object);
    QPainter pd(&down_);
    QPainter pu(&down_);
    bool down = true;
    for (auto layers = items_.begin();layers != items_.end();layers++){
        auto layer = layers->get();
        if (layers->isNull() || layer->hide()) continue;
        if (layer == activeItem_.get()) down = false;
        for (auto draw = layer->items().begin();draw!=layer->items().end();draw++){
            if (draw->isNull()) continue;
            if (down)
                draw->get()->draw(&pd);
            else
                draw->get()->draw(&pu);
        }
    }
    return obj;
}

void LayerManager::switchActiveObject(int index){
    AbstractLayer::switchActiveObject(index);
    QPainter pd(&down_);
    QPainter pu(&down_);
    bool down = true;
    for (auto layers = items_.begin();layers != items_.end();layers++){
        auto layer = layers->get();
        if (layers->isNull() || layer->hide()) continue;
        if (layer == activeItem_.get()) down = false;
        for (auto draw = layer->items().begin();draw!=layer->items().end();draw++){
            if (draw->isNull()) continue;
            if (down)
                draw->get()->draw(&pd);
            else
                draw->get()->draw(&pu);
        }
    }
}

void LayerManager::swap(int a, int b){
    if (a==b) return;
    if (items_.size()<=a||a<0) return;
    if (items_.size()<=b||b<0) return;
    std::swap(items_[a],items_[b]);
}

QPixmap& LayerManager::base(){
    return base_;
}

void LayerManager::setBase(const QSize& baseSize){
    base_ = QPixmap(baseSize);
    base_.fill(QColor(0,0,0,0));
    down_ = QPixmap(base_);
    up_ = QPixmap(base_);
}

void LayerManager::onHideChanged(int i, bool hide){
    if (i<0||i>=items_.size()) return;
    items_.at(i).get()->setHide(hide);
}

QPixmap LayerManager::pixmap(){
    QPixmap pixmap = base_;
    QPainter painter(&pixmap);
    painter.drawPixmap(down_.rect(),down_);
    for (auto draw = activeItem_->items().begin();
        !activeItem_.get()->hide() && draw!=activeItem_->items().end();
        draw++){
        if (draw->isNull()) continue;
        draw->get()->draw(&painter);
    }
    painter.drawPixmap(up_.rect(),up_);
    return pixmap;
}
