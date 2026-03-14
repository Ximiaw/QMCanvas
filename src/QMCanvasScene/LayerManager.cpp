// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "LayerManager.h"
#include "AbstractLayer.h"

template class AbstractLayer<Layer>;

LayerManager::LayerManager(QObject* parent):AbstractLayer(parent){
    base_ = QPixmap(500,500);
    base_.fill(QColor(0,0,0,0));
}

LayerManager::LayerManager(const QSize& baseSize,QObject* parent):AbstractLayer(parent){
    base_ = QPixmap(baseSize);
    base_.fill(QColor(0,0,0,0));
}

bool LayerManager::hide(int index){
    if (items_.size()<=index||index<0) return true;
    return items_.at(index).get()->hide();
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
}

void LayerManager::onHideChanged(int i, bool hide){
    if (i<0||i>=items_.size()) return;
    items_.at(i).get()->setHide(hide);
}

QPixmap LayerManager::pixmap(){
    QPixmap pixmap = base_;
    QPainter painter(&pixmap);
    for (auto layers = items_.begin();layers != items_.end();layers++){
        auto layer = layers->get();
        if (layers->isNull() || layer->hide()) continue;
        for (auto draw = layer->items().begin();draw!=layer->items().end();draw++){
            if (draw->isNull()) continue;
            draw->get()->draw(&painter);
        }
    }
    return pixmap;
}
