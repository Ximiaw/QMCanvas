// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "Layer.h"

Layer::Layer(QObject* parent):QObject(parent){
}

const QList<QMDrawObject*>* Layer::item() const{
    return &items_;
}

QMDrawObject* Layer::activeDrawObject(){
    return activeItem_;
}

void Layer::setActiveDrawObject(QMDrawObject* object){
    if (!object || object == activeItem_) return;
    activeItem_->setParent(nullptr);
    activeItem_->deleteLater();
    activeItem_ = object;
    object->setParent(this);
}

void Layer::finishActiveDrawObject(){
    if (!activeItem_) return;
    items_.append(activeItem_);
    activeItem_ = nullptr;
}

void Layer::undo(){
    if (items_.isEmpty()) return;
    undoStack_.append(items_.takeLast());
}

void Layer::redo(){
    if (undoStack_.isEmpty()) return;
    items_.append(undoStack_.takeLast());
}
