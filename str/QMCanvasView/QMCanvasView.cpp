//
// Created by m3311 on 2026/3/1.
//

#include "QMCanvasView.h"

QMCanvasView::QMCanvasView(QWidget* parent):
    QWidget(parent){
    view_.setParent(this);
    viewport_.setParent(view_.widget());
    connect(this,&QMCanvasView::canvasSceneChanged,&viewport_,&Viewport::onSceneChanged);
}

QMCanvasView::QMCanvasView(QMCanvasScene *scene, QWidget *parent) :
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
        view_.widget()->resize(0,0);
        connect(&view_,&View::viewportChanged,scene,&QMCanvasScene::onViewportChanged);
        connect(&view_,&View::scaleFactorChanged,scene,&QMCanvasScene::onScaleBy);
        connect(this,&QMCanvasView::canvasSceneChanged,scene,&QMCanvasScene::viewportPixmapChanged);
        connect(scene,&QMCanvasScene::viewPropertyChanged,&view_,&View::onPropertyChanged);
        emit canvasSceneChanged(scene);
    }
}

QMCanvasScene* QMCanvasView::canvasScene() const {
    return scenePointer_.get();
}

const Viewport* QMCanvasView::viewport() const {
    return &viewport_;
}
