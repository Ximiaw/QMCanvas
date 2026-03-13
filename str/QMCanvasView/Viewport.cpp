// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "Viewport.h"
#include "QMCanvasScene.h"

Viewport::Viewport(QWidget* parent):
    QWidget(parent){
    setAutoFillBackground(false);
    setMouseTracking(true);
    installEventFilter(this);

}

void Viewport::paintEvent(QPaintEvent* event){
    if (scenePointer_.isNull()) return;
    QPainter painter(this);
    scenePointer_.get()->updatePixmap(&painter);
    scenePointer_.get()->draw(&painter);
    painter.end();
}

bool Viewport::eventFilter(QObject* watched, QEvent* event){
    if (watched == this && event->type() == QEvent::KeyPress && !scenePointer_.isNull()){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        int key = keyEvent->key();

        if (modifiers == Qt::ControlModifier && key == Qt::Key_Z) {
            emit ctrlAndZ();
            return true;
        }
        if (modifiers == Qt::ControlModifier && key == Qt::Key_Y) {
            emit ctrlAndY();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
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
    if (scenePointer_.isNull())
        return;
    if (widget && event->buttons()&Qt::LeftButton && scenePointer_.get()->isMove()){
        emit mouseMove(widget->mapFromGlobal(QCursor::pos()));
    }
    QWidget::mouseMoveEvent(event);
}

void Viewport::mouseReleaseEvent(QMouseEvent* event){
    QWidget* widget=qobject_cast<QWidget*>(parent());
    if (widget && event->button()==Qt::LeftButton){
        emit mouseRelease(widget->mapFromGlobal(QCursor::pos()));
        if (!scenePointer_.isNull())
            scenePointer_.get()->endMove();
    }
    QWidget::mouseReleaseEvent(event);
}

void Viewport::mousePressEvent(QMouseEvent* event){
    QWidget* widget=qobject_cast<QWidget*>(parent());
    if (widget && event->button()==Qt::LeftButton)
    {
        emit mousePress(widget->mapFromGlobal(QCursor::pos()));
        if (!scenePointer_.isNull())
            scenePointer_.get()->beginMove();
    }
    QWidget::mousePressEvent(event);
}