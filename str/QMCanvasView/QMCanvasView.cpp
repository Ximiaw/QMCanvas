//
// Created by m3311 on 2026/3/1.
//

#include "QMCanvasView.h"

QMCanvasView::QMCanvasView(QWidget* parent):
    QWidget(parent){
    view_.setParent(this);
    viewport_.setParent(view_.widget());
    connect(this,&QMCanvasView::canvasSceneChanged,&viewport_,&Viewport::onSceneChanged);

    QVBoxLayout* vl = new QVBoxLayout(this);
    setLayout(vl);
    vl->addWidget(&view_);

    view_.horizontalScrollBar()->installEventFilter(this);
    view_.verticalScrollBar()->installEventFilter(this);
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
        connect(this,&QMCanvasView::canvasSceneChanged,&viewport_,&Viewport::onSceneChanged);
        connect(this,&QMCanvasView::hBarChanged,scene,&QMCanvasScene::onHScrollBarChanged);
        connect(this,&QMCanvasView::vBarChanged,scene,&QMCanvasScene::onVScrollBarChanged);
        connect(scene,&QMCanvasScene::viewportPixmapChanged,&viewport_,&Viewport::onPixmapChanged);
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

bool QMCanvasView::eventFilter(QObject* watched, QEvent* event){
    if (event->type() == QEvent::Wheel) {
        QScrollBar *vBar = view_.verticalScrollBar();
        QScrollBar *hBar = view_.horizontalScrollBar();

        if (watched == vBar) emit vBarChanged(vBar->value());
        if (watched == hBar) emit hBarChanged(hBar->value());
    }

    return QWidget::eventFilter(watched, event);
}
