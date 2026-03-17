// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_ABSTRACTLAYER_H
#define MAIN_ABSTRACTLAYER_H

#include <QObject>
#include <QList>
#include <QPixmap>
#include <QSharedPointer>
#include <QPointer>
#include <QPainter>

template <typename T>
class AbstractLayer:public QObject{
protected:
    QList<QSharedPointer<T>> items_;
    QList<QSharedPointer<T>> undoStack_;
    QPointer<T> activeItem_;
    QPixmap base_;
    QPixmap up_;//活动图层上方的图层合并成的图，减少每帧计算数量
    QPixmap down_;//活动图层下方的图层合并成的图，减少每帧计算数量
    bool hide_;
public:
    AbstractLayer(QObject* parent = nullptr);
    virtual ~AbstractLayer() = default;

    const QList<QSharedPointer<T>>& items() const;

    virtual QPixmap base();
    virtual void setBase(const QSize& baseSize);

    bool hide() const;
    void setHide(bool hide);

    virtual T *activeObject();
    virtual QSharedPointer<T> setActiveObject(QSharedPointer<T> object);
    virtual void switchActiveObject(int index);
    virtual void finishActiveObject();
    
    virtual void undo();
    virtual void redo();

    virtual void update() = 0;
};

#endif //MAIN_ABSTRACTLAYER_H