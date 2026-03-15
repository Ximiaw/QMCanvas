// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_QMDRAWOBJECT_H
#define MAIN_QMDRAWOBJECT_H

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QObject>

class QMDrawObject: public QObject{
protected:
    bool isRecord_ = false;
    QPoint begin_;
    QPoint end_;
    QPoint record_;//当前的位置
public:
    virtual ~QMDrawObject() = default;
    virtual void draw(QPainter* painter) = 0;
    virtual void begin(QPoint point);
    virtual void end(QPoint point);
    virtual void recordPoint (QPoint point);//在begin前禁止记录点，不会触发
    virtual QPen* pen() const = 0;
    virtual QBrush* brush() const = 0;
};


#endif //MAIN_QMDRAWOBJECT_H
