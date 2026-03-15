// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMDrawPixmap.h"

QMDrawPixmap::QMDrawPixmap(QPixmap& pixmap){
    pixmap_ = QPixmap(pixmap);
}

QMDrawPixmap::QMDrawPixmap(){
    pixmap_ = QPixmap(500,500);
    pixmap_.fill(QColor(0,0,0,0));
}

void QMDrawPixmap::setPixmap(QPixmap& pixmap){
    pixmap_ = QPixmap(pixmap);
}

void QMDrawPixmap::draw(QPainter* painter){
    int x = begin_.x();
    int y = begin_.y();
    int w = end_.x() - begin_.x();
    int h = end_.y() - begin_.y();
    painter->drawPixmap(x,y,w,h,pixmap_);
}

QPen* QMDrawPixmap::pen() const{
    return nullptr;
}

QBrush* QMDrawPixmap::brush() const{
    return nullptr;
}
