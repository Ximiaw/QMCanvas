// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef TEST_QMDRAWRECT_H
#define TEST_QMDRAWRECT_H

#include <QMDrawObject.h>

class QMDrawRect:public QMDrawObject
{
private:
    QPen pen_;
    QBrush brush_;
public:
    QMDrawRect(QColor color=Qt::blue, int width=2, bool fill=false);
    void draw(QPainter* painter) override;
    QPen* pen() override;
    QBrush* brush() override;
};


#endif //TEST_QMDRAWRECT_H