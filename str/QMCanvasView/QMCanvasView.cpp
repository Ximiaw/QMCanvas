//
// Created by m3311 on 2026/3/1.
//

#include "QMCanvasView.h"

QMCanvasView::QMCanvasView(QObject* parent):
    QObject(parent){
    connect(this,&QMCanvasView::canvasSceneChanged,&viewport_,&Viewport::onSceneChanged);
}

QMCanvasView::QMCanvasView(QMCanvasScene *scene, QObject *parent) :
    QMCanvasView(parent){
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
        disconnect(scenePointer_.get());
        scenePointer_.reset(scene);
        connect(&view_,&View::viewportChanged,scene,&QMCanvasScene::onViewportChanged);
        connect(&view_,&View::scaleFactorChanged,scene,&QMCanvasScene::onScaleBy);
        emit canvasSceneChanged(scene);
    }
}

QMCanvasScene* QMCanvasView::canvasScene() const {
    return scenePointer_.get();
}

const Viewport* QMCanvasView::viewport() const {
    return &viewport_;
}
