// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_ABSTRACTLAYER_H
#define MAIN_ABSTRACTLAYER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

template <typename T>
class AbstractLayer:public QObject{
protected:
    QList<QSharedPointer<T>> items_;
    QList<QSharedPointer<T>> undoStack_;
    QSharedPointer<T> activeItem_;
    bool hide_;
public:
    AbstractLayer(QObject* parent = nullptr);
    virtual ~AbstractLayer() = default;

    const QList<QSharedPointer<T>>& items() const;

    bool hide() const;
    void setHide(bool hide);

    virtual T *activeObject();
    virtual QSharedPointer<T> setActiveObject(QSharedPointer<T> object);
    virtual void switchActiveObject(int index);

    void finishActiveObject();

    virtual void undo();
    virtual void redo();
};

#endif //MAIN_ABSTRACTLAYER_H