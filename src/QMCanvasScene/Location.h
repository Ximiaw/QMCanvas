// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_TRANSFORM_H
#define MAIN_TRANSFORM_H

#include <QRectF>
#include <QPoint>

class Location
{
private:
    QRectF baseRect_;//基础区域，所有位置、空间都基于这个，相当于未缩放的图片大小
    QRectF viewportRect_;//未缩放未外扩的viewport区域
    //QRectF viewportRectRM_;//缩放外扩后的viewport区域

    bool considerMouse_;//为true时根据鼠标计算偏移
    QPoint mousePoint_;//鼠标指向的位置，是未缩放未外扩的

    qreal factor_=1.0;//每次缩放倍率，和viewportRate相乘
    qreal ratio_=1.0;//当前图像倍率，如1.2为原图的1.2倍
    qreal marginRate_=1.0;//当前外扩倍率，1为默认值
    qreal maxRatio_=20.0;//最大缩放倍率
    qreal minRatio_=0.01;//最小缩放倍率
public:
    Location(QRectF baseRect);
    ~Location()=default;

    QRectF baseRect();
    void setBaseRect(QRectF baseRect);

    QRectF viewportRect();
    void setViewportRect(QRectF rect);

    QRectF viewportRectRM();
    void setMousePoint(QPoint point);

    qreal maxRatio() const;
    void setMaxRatio(qreal max);

    qreal minRatio() const;
    void setMinRatio(qreal min);

    qreal factor() const;
    void setFactor(qreal factor);

    qreal extraViewportMargin() const;
    void setExtraViewportMargin(qreal rate);

    qreal ratio() const;
    void setRatio(qreal ratio);
};


#endif //MAIN_TRANSFORM_H