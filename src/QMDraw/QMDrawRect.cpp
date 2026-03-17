// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMDrawRect.h"

QMDrawRect::QMDrawRect(QColor color, int width, bool fill){
    pen_ = QPen(color,width);
    brush_ = fill ? QBrush(color.lighter(180)) : QBrush(Qt::NoBrush);
    fill_ = fill;
}

void QMDrawRect::draw(QPainter* painter){
    QPen p = painter->pen();
    QBrush b = painter->brush();

    painter->setPen(pen_);
    int x = begin_.x() < end_.x() ? begin_.x() : end_.x();
    int y = begin_.y() < end_.y() ? begin_.y() : end_.y();
    int w = end_.x() - begin_.x();
    w = w > 0 ? w : -w;
    int h = end_.y() - begin_.y();
    h = h > 0 ? h : -h;
    painter->drawRect(x,y,w,h);

    painter->setPen(p);
    painter->setBrush(b);
}

QPen* QMDrawRect::pen(){
    return &pen_;
}

QBrush* QMDrawRect::brush(){
    return &brush_;
}

QSharedPointer<QMDrawObject> QMDrawRect::clone(){
    return QSharedPointer<QMDrawObject>(new QMDrawRect(pen_.color(),pen_.width(),fill_));
}
