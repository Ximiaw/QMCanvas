//
// Created by m3311 on 2026/3/1.
//

#include "Viewport.h"
#include "QMCanvasScene.h"

Viewport::Viewport(QWidget* parent):
    QWidget(parent){
    setAutoFillBackground(false);
    setMouseTracking(true);
    setStyleSheet("background-color: red;");//测试用，方便观察，回头记得删
}

void Viewport::paintEvent(QPaintEvent* event){
    if (scenePointer_.isNull()) return;
    QPainter painter(this);
    scenePointer_.get()->updatePixmap(&painter);
    scenePointer_.get()->draw(&painter);
    painter.end();
}

void Viewport::onSceneChanged(QMCanvasScene* scene){
    if (scenePointer_.get() != scene){
        if (!scenePointer_.isNull())
            disconnect(scenePointer_.get());
        scenePointer_ = QPointer<QMCanvasScene>(scene);
        onRectChanged();
    }
}

void Viewport::onRectChanged(){
    if (scenePointer_.isNull()) return;
    QRect rect = scenePointer_.get()->getViewportRect();
    if (geometry() != rect){
        setGeometry(rect);
    }
    update();
}

void Viewport::onPixmapChanged(){
    if (scenePointer_.isNull()) return;
    update();
}

void Viewport::mouseMoveEvent(QMouseEvent* event){
    QWidget* widget=qobject_cast<QWidget*>(parent());
    if (widget && event->buttons()&Qt::LeftButton)
        emit mouseMove(widget->mapFromGlobal(QCursor::pos()));
    QWidget::mouseMoveEvent(event);
}

void Viewport::mouseReleaseEvent(QMouseEvent* event){
    QWidget* widget=qobject_cast<QWidget*>(parent());
    if (widget && event->buttons()&Qt::LeftButton)
        emit mouseRelease(widget->mapFromGlobal(QCursor::pos()));
    QWidget::mouseReleaseEvent(event);
}

void Viewport::mousePressEvent(QMouseEvent* event){
    QWidget* widget=qobject_cast<QWidget*>(parent());
    if (widget && event->buttons()&Qt::LeftButton)
        emit mousePress(widget->mapFromGlobal(QCursor::pos()));
    QWidget::mousePressEvent(event);
}