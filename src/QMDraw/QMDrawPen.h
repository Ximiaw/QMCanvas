// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef TEST_QMDRAWPEN_H
#define TEST_QMDRAWPEN_H

#include <QLine>

#include "QMDrawObject.h"

class QMDrawPen:public QMDrawObject
{
private:
    QList<QPoint> points_;
    QPen pen_;
public:
    QMDrawPen(QColor color=Qt::black,int width=3);
    void draw(QPainter* painter) override;
    void begin(QPoint point) override;
    void recordPoint(QPoint point) override;
    void end(QPoint point) override;
    QPen* pen() override;
    QBrush* brush() override;
};


#endif //TEST_QMDRAWPEN_H