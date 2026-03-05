//
// Created by m3311 on 2026/3/5.
//

#ifndef MAIN_QMDRAWOBJECT_H
#define MAIN_QMDRAWOBJECT_H

#include <QObject>
#include <QPainter>

class QMDrawObject : public QObject{
    Q_OBJECT
public:
    QMDrawObject(QObject* parent);
    ~QMDrawObject() override = default;
    virtual void draw(QPainter* painter) = 0;
};


#endif //MAIN_QMDRAWOBJECT_H
