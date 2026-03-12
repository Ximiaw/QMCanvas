//
// Created by m3311 on 2026/3/5.
//

#ifndef MAIN_QMDRAWOBJECT_H
#define MAIN_QMDRAWOBJECT_H

#include <QObject>
#include <QPainter>
#include <QPen>
#include <QBrush>

class QMDrawObject : public QObject{
    Q_OBJECT
private:
    bool record = false;
public:
    QMDrawObject(QObject* parent);
    ~QMDrawObject() override = default;
    virtual void draw(QPainter* painter) = 0;
    virtual void begin(QPoint point) = 0;
    virtual void end(QPoint point) = 0;
    virtual void recordPoint (QPoint point) = 0;//在begin前禁止记录点，不会触发
    virtual QPen* pen() const = 0;
    virtual QBrush* brush() const = 0;
};


#endif //MAIN_QMDRAWOBJECT_H
