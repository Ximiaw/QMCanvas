// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMDrawPen.h"

QMDrawPen::QMDrawPen(QColor color, int width){
    pen_ = QPen(color,width,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}

void QMDrawPen::draw(QPainter* painter){
    if (points_.size()<2) return;
    QPen p = painter->pen();
    painter->setPen(pen_);
    for (int i=0;i<points_.size()-1;i++){
        painter->drawLine(points_[i],points_[i+1]);
    }
    painter->setPen(p);
}

void QMDrawPen::begin(QPoint point){
    QMDrawObject::begin(point);
    points_.append(point);
}

void QMDrawPen::recordPoint(QPoint point){
    if (isRecord_){
        record_ = point;
        points_.append(record_);
    }
}

void QMDrawPen::end(QPoint point){
    if (isRecord_){
        isRecord_ = false;
        end_ = point;
        points_.append(end_);
    }
}

QPen* QMDrawPen::pen(){
    return &pen_;
}

QBrush* QMDrawPen::brush(){
    return nullptr;
}

QSharedPointer<QMDrawObject> QMDrawPen::clone(){
    return QSharedPointer<QMDrawObject>(new QMDrawPen(pen_.color(),pen_.width()));
}
