// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#ifndef MAIN_QMCANVASVIEW_H
#define MAIN_QMCANVASVIEW_H

#include <QObject>
#include <QSharedPointer>
#include <QWidget>
#include <QBoxLayout>
#include <QScrollBar>

#include "Viewport.h"
#include "View.h"
#include "QMCanvasScene.h"

class QMCanvasView : public QWidget{
    Q_OBJECT
    Q_ENUM(WheelMode)
    Q_PROPERTY(WheelMode wheelMode READ wheelMode WRITE setWheelMode)
    Q_PROPERTY(const Viewport* viewport READ viewport)
    Q_PROPERTY(QMCanvasScene* canvasScene READ canvasScene WRITE setCanvasScene NOTIFY canvasSceneChanged)

public:
    QMCanvasView(QWidget* parent=nullptr);
    QMCanvasView(QMCanvasScene* scene,QWidget* parent=nullptr);

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

protected:
    void resizeEvent(QResizeEvent* event) override;

signals:
    void canvasSceneChanged(QMCanvasScene* scene);
    void sizeChanged(QSize size);
};


#endif //MAIN_QMCANVASVIEW_H