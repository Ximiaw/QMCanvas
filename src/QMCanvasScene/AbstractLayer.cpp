// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "AbstractLayer.h"
#include "Layer.h"
#include "QMDrawObject.h"

template <typename T>
AbstractLayer<T>::AbstractLayer(QObject* parent):QObject(parent){
    hide_=false;
    activeItem_ = nullptr;
}

template <typename T>
const QList<QSharedPointer<T>>& AbstractLayer<T>::items() const{
    return items_;
}

template <typename T>
QPixmap AbstractLayer<T>::base(){
    return base_;
}

template <typename T>
void AbstractLayer<T>::setBase(const QSize& baseSize){
    base_ = QPixmap(baseSize);
    base_.fill(QColor(0,0,0,0));
    down_ = QPixmap(base_);
    up_ = QPixmap(base_);
}

template <typename T>
bool AbstractLayer<T>::hide() const{
    return hide_;
}

template <typename T>
void AbstractLayer<T>::setHide(bool hide){
    hide_=hide;
}

template <typename T>
T* AbstractLayer<T>::activeObject(){
    return activeItem_.get();
}

template <typename T>
QSharedPointer<T> AbstractLayer<T>::setActiveObject(QSharedPointer<T> object){
    if (object.isNull() || object.get() == activeItem_.get()) return nullptr;
    QSharedPointer<T> n;
    if (!activeItem_.isNull()){
        auto it = std::find_if(items_.begin(),items_.end(),
            [this](const QSharedPointer<T>& sp) { return sp.get() == activeItem_.get();});
        if (it!=items_.end()){
            n = *it;
            items_.erase(it);
        }
    }
    if (items_.contains(object)) items_.removeOne(object);
    activeItem_ = QPointer<T>(object.get());
    items_.append(object);
    return n;
}

template <typename T>
void AbstractLayer<T>::switchActiveObject(int index){
    if (index<0||items_.size()<=index) return;
    activeItem_ = QPointer<T>(items_.at(index).get());
}

template <typename T>
void AbstractLayer<T>::finishActiveObject(){
    if (activeItem_.isNull()) return;
    activeItem_ = QPointer<T>(nullptr);
    undoStack_.clear();
}

template <typename T>
void AbstractLayer<T>::undo(){
    if (items_.isEmpty()) return;
    undoStack_.append(items_.takeLast());
    activeItem_ = QPointer<T>(nullptr);
}

template <typename T>
void AbstractLayer<T>::redo(){
    if (undoStack_.isEmpty()) return;
    items_.append(undoStack_.takeLast());
    activeItem_ = QPointer<T>(items_.back().get());
}

template class AbstractLayer<QMDrawObject>;
template class AbstractLayer<Layer>;