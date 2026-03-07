//
// Created by m3311 on 2026/3/1.
//

#ifndef MAIN_QMCANVASVIEW_H
#define MAIN_QMCANVASVIEW_H

#include <QObject>
#include <QSharedPointer>

#include "Viewport.h"
#include "View.h"
#include "QMCanvasScene.h"

class QMCanvasView : public QObject{
    Q_OBJECT
    Q_ENUM(WheelMode)
    Q_PROPERTY(WheelMode wheelMode READ wheelMode WRITE setWheelMode NOTIFY wheelModeChanged)
    Q_PROPERTY(const Viewport* viewport READ viewport)
    Q_PROPERTY(QMCanvasScene* canvasScene READ canvasScene WRITE setCanvasScene NOTIFY canvasSceneChanged)

public:
    QMCanvasView(QObject* parent=nullptr);
    QMCanvasView(QMCanvasScene* scene,QObject* parent=nullptr);

private:
    View view_;
    Viewport viewport_;
    QSharedPointer<QMCanvasScene> scenePointer_;

public:
    void setWheelMode(WheelMode mode);
    WheelMode wheelMode() const;
    void setCanvasScene(QMCanvasScene* scene);
    QMCanvasScene* canvasScene() const;
    const Viewport* viewport() const;

signals:
    void canvasSceneChanged(QMCanvasScene* scene);
};


#endif //MAIN_QMCANVASVIEW_H