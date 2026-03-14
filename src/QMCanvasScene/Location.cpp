// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "Location.h"

Location::Location(QRectF baseRect){
    setBaseRect(baseRect);
}

QRectF Location::baseRect(){
    return baseRect_;
}

void Location::setBaseRect(QRectF baseRect){
    baseRect_=baseRect;
}

QRectF Location::viewportRect(){
    return viewportRect_;
}

void Location::setViewportRect(QRectF rect){
    viewportRect_=rect;
}

QRectF Location::viewportRectRM(){
    qreal rx = viewportRect_.x() * ratio();
    qreal ry = viewportRect_.y() * ratio();
    qreal rmw = viewportRect_.width() * ratio() * extraViewportMargin();
    qreal rmh = viewportRect_.height() * ratio() * extraViewportMargin();

    qreal dw = (rmw - viewportRect_.width() * ratio()) / 2;
    qreal dh = (rmh - viewportRect_.height() * ratio()) / 2;

    rx = rx - dw;
    ry = ry - dh;

    QRectF rect(rx,ry,rmw,rmh);

    if (considerMouse_){
        qreal m_rx = mousePoint_.x() * ratio();
        qreal m_ry = mousePoint_.y() * ratio();

        qreal m_v_dx = mousePoint_.x() - viewportRect_.x();
        qreal m_v_dy = mousePoint_.y() - viewportRect_.y();

        qreal x = m_rx - m_v_dx;
        qreal y = m_ry - m_v_dy;

        rect.setX(x);
        rect.setY(y);

        considerMouse_=false;
    }

    qreal qw = baseRect_.width() * ratio();
    qreal qh = baseRect_.height() * ratio();

    if (qw < rect.x() + rect.width()){
        rect.setX(qw - rmw);
    }else if (rect.x() < 0){
        rect.setX(0);
    }
    if (qh < rect.y() + rect.height()){
        rect.setY(qh - rmh);
    }else if (rect.y() < 0){
        rect.setY(0);
    }

    return rect.toRect();
}

void Location::setMousePoint(QPoint point){
    mousePoint_ = QPoint(point);
    considerMouse_=true;
}

qreal Location::maxRatio() const{
    return maxRatio_;
}

void Location::setMaxRatio(qreal max){
    maxRatio_=max;
}

qreal Location::minRatio() const{
    return minRatio_;
}

void Location::setMinRatio(qreal min){
    minRatio_=min;
}

qreal Location::factor() const{
    return factor_;
}

void Location::setFactor(qreal factor){
    factor_=factor;
}

qreal Location::extraViewportMargin() const{
    return marginRate_;
}

void Location::setExtraViewportMargin(qreal rate){
    marginRate_=rate;
}

qreal Location::ratio() const{
    return ratio_;
}

void Location::setRatio(qreal ratio){
    ratio_=ratio;
}
