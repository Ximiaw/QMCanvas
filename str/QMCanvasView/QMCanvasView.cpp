//
// Created by m3311 on 2026/3/1.
//

#include "QMCanvasView.h"

QMCanvasView::QMCanvasView(QObject* parent):
    QObject(parent) {}

QMCanvasView::QMCanvasView(QMCanvasScene *scene, QObject *parent) :
    QMCanvasView(parent){
    scenePointer_.reset(scene);
}

void QMCanvasView::setWheelMode(WheelMode mode) {
    if (view_.wheelMode()!=mode) {
        view_.setWheelMode(mode);
        emit wheelModeChanged(mode);
    }
}

WheelMode QMCanvasView::wheelMode() const {
    return view_.wheelMode();
}

void QMCanvasView::setCanvasScene(QMCanvasScene* scene) {
    if (scenePointer_.get()!=scene) {
        scenePointer_.reset(scene);
        emit canvasSceneChanged(scene);
    }
}

QMCanvasScene* QMCanvasView::canvasScene() const {
    return scenePointer_.get();
}

const Viewport* QMCanvasView::viewport() const {
    return &viewport_;
}
