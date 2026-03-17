// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMCanvasView.h"

QMCanvasView::QMCanvasView(QWidget* parent):
    QWidget(parent){
    view_.setParent(this);
    viewport_.setParent(view_.widget());

    QVBoxLayout* vl = new QVBoxLayout(this);
    setLayout(vl);
    vl->addWidget(&view_);
}

QMCanvasView::QMCanvasView(QMCanvasScene *scene, QWidget *parent) :
    QWidget(parent){
    view_.setParent(this);
    viewport_.setParent(view_.widget());
    connect(this,&QMCanvasView::canvasSceneChanged,&viewport_,&Viewport::onSceneChanged);

    QVBoxLayout* vl = new QVBoxLayout(this);
    setLayout(vl);
    vl->addWidget(&view_);
    setCanvasScene(scene);
}

void QMCanvasView::setWheelMode(WheelMode mode) {
    if (view_.wheelMode()!=mode) {
        view_.setWheelMode(mode);
    }
}

WheelMode QMCanvasView::wheelMode() const {
    return view_.wheelMode();
}

void QMCanvasView::setCanvasScene(QMCanvasScene* scene) {
    if (scenePointer_.get()!=scene) {
        if (!scenePointer_.isNull())
            disconnect(scenePointer_.get());
        scenePointer_.reset(scene);
        view_.widget()->resize(0,0);
        scene->init(this,&view_,&viewport_);

        emit canvasSceneChanged(scene);
    }
}

QMCanvasScene* QMCanvasView::canvasScene() const {
    return scenePointer_.get();
}

Viewport* QMCanvasView::viewport(){
    return &viewport_;
}

void QMCanvasView::updateViewport(){
    viewport_.update();
}

void QMCanvasView::setViewportBackground(QColor color){
    QPalette palette = viewport_.palette();
    palette.setColor(QPalette::Window, color);
    viewport_.setPalette(palette);
    viewport_.setAutoFillBackground(true);
}

void QMCanvasView::resizeEvent(QResizeEvent* event){
    QWidget::resizeEvent(event);
    emit sizeChanged(viewport()->size());
}

void QMCanvasView::keyPressEvent(QKeyEvent* event){
    if (event->key() == Qt::Key_Control){
        setWheelMode(WheelMode::ZOOM);
    }
    QWidget::keyPressEvent(event);
}

void QMCanvasView::keyReleaseEvent(QKeyEvent* event){
    if (event->key() == Qt::Key_Control){
        setWheelMode(WheelMode::ROLL);
    }
    QWidget::keyReleaseEvent(event);
}
