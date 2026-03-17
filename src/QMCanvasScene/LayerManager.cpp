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

void LayerManager::setHide(int index, bool hide){
    if (index < 0 || index >= items_.size()) return;
    items_.at(index).get()->setHide(hide);
    update();
}

QSharedPointer<Layer> LayerManager::setActiveObject(QSharedPointer<Layer> object){
    auto obj = AbstractLayer::setActiveObject(object);
    down_ = QPixmap(base_);
    QPainter pd(&down_);
    object->setBase(base_.size());
    for (auto layers = items_.begin();layers != items_.end();layers++){
        auto layer = layers->get();
        if (layers->isNull() || layer->hide() || layer == object.get()) continue;
        pd.drawPixmap(layer->pixmap().rect(),layer->pixmap());
    }
    return obj;
}

void LayerManager::switchActiveObject(int index){
    AbstractLayer::switchActiveObject(index);
    update();
}

void LayerManager::finishActiveObject(){
    if (activeItem_.isNull() || !activeItem_->activeObject()) return;
    auto ptr = activeItem_->activeObject()->clone();
    AbstractLayer::finishActiveObject();
    auto layer_ptr = new Layer;
    layer_ptr->setBase(base_.size());
    setActiveObject(QSharedPointer<Layer>(layer_ptr));
    activeItem_->setActiveObject(QSharedPointer<QMDrawObject>(ptr));
}

void LayerManager::swap(int a, int b){
    if (a==b) return;
    if (items_.size()<=a||a<0) return;
    if (items_.size()<=b||b<0) return;
    std::swap(items_[a],items_[b]);
}

void LayerManager::onHideChanged(int i, bool hide){
    if (i<0||i>=items_.size()) return;
    items_.at(i).get()->setHide(hide);
}

QPixmap LayerManager::pixmap(){
    if (activeItem_.isNull() || activeItem_->items().isEmpty()) return base_;
    QPixmap pixmap = base_;
    QPainter painter(&pixmap);
    painter.drawPixmap(down_.rect(),down_);
    if (!activeItem_.isNull() && !activeItem_->hide())
        painter.drawPixmap(activeItem_->pixmap().rect(),activeItem_->pixmap());
    painter.drawPixmap(up_.rect(),up_);
    return pixmap;
}

void LayerManager::update(){
    setBase(base_.size());
    QPainter pd(&down_);
    QPainter pu(&up_);
    bool down = true;
    for (auto layers = items_.begin();layers != items_.end();layers++){
        auto layer = layers->get();
        if (layers->isNull() || layer->hide()) continue;
        if (layer == activeItem_.get()) down = false;
        if (down)
            pd.drawPixmap(layer->pixmap().rect(),layer->pixmap());
        else
            pu.drawPixmap(layer->pixmap().rect(),layer->pixmap());
    }
}
