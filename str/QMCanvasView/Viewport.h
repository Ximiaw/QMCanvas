//
// Created by m3311 on 2026/3/1.
//

#ifndef MAIN_VIEWPORT_H
#define MAIN_VIEWPORT_H

#include <QObject>
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QPaintEvent>
#include <QPointer>

#include "QMCanvasScene.h"

class Viewport :public QOpenGLWidget{
    Q_OBJECT
private:
    QPointer<QMCanvasScene> scenePointer_;
public:
    Viewport(QWidget* parent=nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
public slots:
    void onSceneChanged(QMCanvasScene* scene);
};

#endif //MAIN_VIEWPORT_H