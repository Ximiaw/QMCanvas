// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_LAYER_H
#define MAIN_LAYER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "QMDrawObject.h"

class Layer : public QObject
{
    Q_OBJECT
private:
    QList<QMDrawObject*> items_;
    QList<QMDrawObject*> undoStack_;
    QMDrawObject* activeItem_;
public:
    Layer(QObject* parent=nullptr);
    ~Layer() override = default;

    const QList<QMDrawObject*> *item() const;

    QMDrawObject *activeDrawObject();
    void setActiveDrawObject(QMDrawObject* object);//会delete旧的活跃绘图对象
    void finishActiveDrawObject();

    void undo();
    void redo();
};


#endif //MAIN_LAYER_H