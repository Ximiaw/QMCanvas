// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "AbstractLayer.h"

template <typename T>
AbstractLayer<T>::AbstractLayer(QObject* parent):QObject(parent){
    hide_=false;
    activeItem_.reset(nullptr);
}

template <typename T>
const QList<QSharedPointer<T>>& AbstractLayer<T>::items() const{
    return items_;
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
    if (object.isNull() || object.get() == activeItem_.get()) return QSharedPointer<T>{nullptr};
    if (items_.back().get()==activeItem_.get()) items_.takeLast();
    auto n = activeItem_;
    activeItem_ = object;
    items_.append(activeItem_);
    return n;
}

template <typename T>
void AbstractLayer<T>::setActiveObject(int index){
    if (index<0||items_.size()<=index) return;
    activeItem_ = items_.at(index);
}

template <typename T>
void AbstractLayer<T>::finishActiveObject()
{
    if (activeItem_.isNull()) return;
    activeItem_.reset(nullptr);
    undoStack_.clear();
}

template <typename T>
void AbstractLayer<T>::undo(){
    if (items_.isEmpty()) return;
    undoStack_.append(items_.takeLast());
}

template <typename T>
void AbstractLayer<T>::redo(){
    if (undoStack_.isEmpty()) return;
    items_.append(undoStack_.takeLast());
}