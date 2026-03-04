//
// Created by m3311 on 2026/3/1.
//

#include "QMCanvasView.h"

QMCanvasView::QMCanvasView(QObject* parent):
    QObject(parent){}

void QMCanvasView::setWheelMode(WheelMode mode) {
    view_.setWheelMode(mode);
}

WheelMode QMCanvasView::wheelMode() const {
    return view_.wheelMode();
}

void QMCanvasView::setCanvasScene(QMCanvasScene* scene) {
    scenePointer_.reset(scene);
}

QMCanvasScene* QMCanvasView::canvasScene() const {
    return scenePointer_.get();
}

const Viewport* QMCanvasView::viewport() const {
    return &viewport_;
}
