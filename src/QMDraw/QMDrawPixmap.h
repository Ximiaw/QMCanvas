// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_QMDRAWPIXMAP_H
#define MAIN_QMDRAWPIXMAP_H

#include <QPixmap>
#include "QMDrawObject.h"

class QMDrawPixmap:public QMDrawObject
{
private:
    QPixmap pixmap_;
    bool fill_;
public:
    QMDrawPixmap(QPixmap& pixmap);
    QMDrawPixmap();

    bool fill();
    void setFill(bool fill);

    void setPixmap(QPixmap& pixmap);

    void draw(QPainter* painter) override;
    QPen* pen() override;
    QBrush* brush() override;
    QSharedPointer<QMDrawObject> clone() override;
};


#endif //MAIN_QMDRAWPIXMAP_H