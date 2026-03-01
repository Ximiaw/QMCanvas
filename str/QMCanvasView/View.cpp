//
// Created by m3311 on 2026/2/28.
//

#include "QMCanvasView.h"
#include "QMCanvasScene.h"

QMCanvasView::QMCanvasView(QWidget *parent):QScrollArea(parent) {
    viewportPointer_.reset(new Viewport(this));
}

QMCanvasView::QMCanvasView(QMCanvasScene *scene, QWidget *parent)
    :QMCanvasView(parent){
    setCanvasScene(scene);
}

QMCanvasView::~QMCanvasView() = default;

const QMCanvasView::Viewport* QMCanvasView::viewport() const {
    return  viewportPointer_.get();
}

void QMCanvasView::setWheelMode(WheelMode mode) {
    if (wheelMode_!=mode) {
        wheelMode_=mode;
        emit wheelModeChanged(mode);
    }
}
WheelMode QMCanvasView::wheelMode() const {
    return wheelMode_;
}

void QMCanvasView::setCanvasScene(QMCanvasScene* scene) {
    if (scenePointer_.get()!=scene) {
        scenePointer_.reset(scene);
        emit canvasSceneChanged(scene);
    }
}
QMCanvasScene *QMCanvasView::canvasScene() const {
    return scenePointer_.get();
}

void QMCanvasView::wheelEvent(QWheelEvent *event) {
    switch (wheelMode()) {
        case WheelMode::ROLL:
            QScrollArea::wheelEvent(event);
            break;
        case WheelMode::ZOOM:
            renderScaled(event);
            break;
    }
}

void QMCanvasView::renderScaled(QWheelEvent *event) {
    //
}
