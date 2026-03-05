//
// Created by m3311 on 2026/3/1.
//

#ifndef MAIN_QMCANVASSCENE_H
#define MAIN_QMCANVASSCENE_H

#include <QObject>
#include <QList>

#include "QMDrawObject.h"

class QMCanvasScene : public QObject {
    Q_OBJECT
private:
    QList<QMDrawObject*> drawObject_;
public:
    QMCanvasScene(QObject* parent);
    ~QMCanvasScene() override = default;

    const QList<QMDrawObject*> graphicList();
    void addGraphic(QMDrawObject* graphic);
    void removeGraphic(QMDrawObject* graphic);
};


#endif //MAIN_QMCANVASSCENE_H