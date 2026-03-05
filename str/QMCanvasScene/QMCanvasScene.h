//
// Created by m3311 on 2026/3/1.
//

#ifndef MAIN_QMCANVASSCENE_H
#define MAIN_QMCANVASSCENE_H

#include <QObject>
#include <QList>
#include <QRectF>

#include "QMDrawObject.h"

class QMCanvasScene : public QObject {
    Q_OBJECT
private:
    QList<QMDrawObject*> drawObject_;
    qreal factor_=1.0;//每次缩放倍率
    qreal rate_=1;//1为默认值
public:
    QMCanvasScene(QObject* parent);
    ~QMCanvasScene() override = default;

    const QList<QMDrawObject*> graphicList();
    void addGraphic(QMDrawObject* graphic);//添加到对象树
    void removeGraphic(QMDrawObject* graphic);
    void deleteGraphic(QMDrawObject* graphic);

    qreal factor();
    void setFactor(qreal factor);
    qreal extraViewportMargin() const;
    void setExtraViewportMargin(qreal rate);

public slots:
    void onViewportChanged(QRectF rect);//需要在这里处理外扩
    void onScaleBy(bool magnify,QPoint point);
};


#endif //MAIN_QMCANVASSCENE_H