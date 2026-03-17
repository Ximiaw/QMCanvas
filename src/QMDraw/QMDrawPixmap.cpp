// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMDrawPixmap.h"

QMDrawPixmap::QMDrawPixmap(QPixmap& pixmap){
    pixmap_ = QPixmap(pixmap);
    fill_ = false;
}

QMDrawPixmap::QMDrawPixmap(){
    pixmap_ = QPixmap(500,500);
    pixmap_.fill(QColor(0,0,0,0));
    fill_ = false;
}

bool QMDrawPixmap::fill(){
    return fill_;
}

void QMDrawPixmap::setFill(bool fill){
    fill_ = fill;
}

void QMDrawPixmap::setPixmap(QPixmap& pixmap){
    pixmap_ = QPixmap(pixmap);
}

void QMDrawPixmap::draw(QPainter* painter){
    if (fill_){
        painter->drawPixmap(pixmap_.rect(),pixmap_);
        return;
    }
    int x = begin_.x() < end_.x() ? begin_.x() : end_.x();
    int y = begin_.y() < end_.y() ? begin_.y() : end_.y();
    int w = end_.x() - begin_.x();
    w = w > 0 ? w : -w;
    int h = end_.y() - begin_.y();
    h = h > 0 ? h : -h;
    painter->drawPixmap(x,y,w,h,pixmap_);
}

QPen* QMDrawPixmap::pen(){
    return nullptr;
}

QBrush* QMDrawPixmap::brush(){
    return nullptr;
}

QMDrawObject* QMDrawPixmap::clone(){
    auto ptr = new QMDrawPixmap(pixmap_);
    ptr->setFill(fill_);
    return ptr;
}
