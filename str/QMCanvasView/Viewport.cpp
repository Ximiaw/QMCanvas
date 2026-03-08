//
// Created by m3311 on 2026/3/1.
//

#include "Viewport.h"
#include "QMCanvasScene.h"

Viewport::Viewport(QWidget* parent):
    QWidget(parent){
    setAutoFillBackground(false);
    setMouseTracking(true);
}

void Viewport::paintEvent(QPaintEvent* event){
    if (scenePointer_.isNull()) return;
    QPainter painter(this);
    if (!updatePixmap_){
        scenePointer_.get()->updatePixmap(&painter);
        updatePixmap_=true;
    }
    scenePointer_.get()->draw(&painter);
}

void Viewport::onSceneChanged(QMCanvasScene* scene){
    if (scenePointer_.get() != scene){
        updatePixmap_=false;
        if (!scenePointer_.isNull())
            disconnect(scenePointer_.get());
        scenePointer_ = QPointer<QMCanvasScene>(scene);
        connect(this,&Viewport::mouseMove,scene,&QMCanvasScene::onMouseMove);
        connect(this,&Viewport::mouseRelease,scene,&QMCanvasScene::onMouseRelease);
        connect(this,&Viewport::mousePress,scene,&QMCanvasScene::onMousePress);
        connect(scene,&QMCanvasScene::viewportRectChanged,this,&Viewport::onRectChanged);
        connect(scene,&QMCanvasScene::viewportPixmapChanged,this,&Viewport::onPixmapChanged);
        onRectChanged();
    }
}

void Viewport::onRectChanged(){
    if (scenePointer_.isNull()) return;
    QRect rect = scenePointer_.get()->getViewportRect();
    if (geometry() != rect){
        setGeometry(rect);
    }
}

void Viewport::onPixmapChanged(){
    updatePixmap_=false;
}

void Viewport::mouseMoveEvent(QMouseEvent* event){
    QWidget* widget=qobject_cast<QWidget*>(parent());
    if (widget && event->button()==Qt::LeftButton)
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
    if (widget && event->button()==Qt::LeftButton)
        emit mousePress(widget->mapFromGlobal(QCursor::pos()));
    QWidget::mousePressEvent(event);
}