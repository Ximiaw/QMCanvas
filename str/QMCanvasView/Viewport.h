//
// Created by m3311 on 2026/3/1.
//

#ifndef MAIN_VIEWPORT_H
#define MAIN_VIEWPORT_H

#include <QObject>
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QPaintEvent>

#include "QMCanvasScene.h"

class Viewport :public QOpenGLWidget{
    Q_OBJECT
private:
    QMCanvasScene* scene_;
public:
    Viewport(QWidget* parent=nullptr);
    Viewport(QMCanvasScene* scene,QWidget* parent);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif //MAIN_VIEWPORT_H