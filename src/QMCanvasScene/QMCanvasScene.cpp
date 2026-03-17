// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMCanvasScene.h"
#include "QMCanvasView.h"
#include "Viewport.h"
#include "View.h"

QMCanvasScene::QMCanvasScene(QPixmap pixmap,QObject* parent)
    :QObject(parent)
    ,location_(pixmap.rect())
    ,layerManager_(pixmap.size()){
    layerManager_.activeObject()->setActiveObject(QSharedPointer<QMDrawObject>(new QMDrawPixmap(pixmap)));
}

bool QMCanvasScene::isMove() const{
    return mouseMove_;
}

void QMCanvasScene::beginMove(){
    mouseMove_ = true;
}

void QMCanvasScene::endMove(){
    mouseMove_ = false;
}

qreal QMCanvasScene::maxRatio() const{
    return location_.maxRatio();
}

void QMCanvasScene::setMaxRatio(qreal max){
    location_.setMaxRatio(max);
}

qreal QMCanvasScene::minRatio() const{
    return location_.minRatio();
}

void QMCanvasScene::setMinRatio(qreal min){
    location_.setMinRatio(min);
}

const QPixmap QMCanvasScene::pixmap(){
    return layerManager_.pixmap();
}

QPixmap QMCanvasScene::getViewportPixmap(){
    QRectF rect = location_.viewportRect();
    QSize size = location_.viewportRectRM().toRect().size();
    QPixmap viewportPixmap = pixmap().copy(rect.toRect())
        .scaled(size,Qt::KeepAspectRatio, Qt::FastTransformation);
    return viewportPixmap;
}

QRect QMCanvasScene::getViewportRect(){
    return location_.viewportRectRM().toRect();
}

void QMCanvasScene::draw(QPainter* painter){
    QPixmap pixmap = getViewportPixmap();
    QRect rect = pixmap.rect();
    painter->drawPixmap(rect,pixmap);
}

void QMCanvasScene::init(QMCanvasView* canvasView,View* view,Viewport* viewport){
    view->widget()->setGeometry(location_.viewportRect().toRect());
    location_.setViewportRect(view->viewport()->size());

    connect(viewport,&Viewport::mouseMove,this,&QMCanvasScene::onMouseMove);
    connect(viewport,&Viewport::mouseRelease,this,&QMCanvasScene::onMouseRelease);
    connect(viewport,&Viewport::mousePress,this,&QMCanvasScene::onMousePress);
    connect(viewport,&Viewport::ctrlAndZ,this,&QMCanvasScene::onCtrlAndZ);
    connect(viewport,&Viewport::ctrlAndY,this,&QMCanvasScene::onCtrlAndY);
    connect(this,&QMCanvasScene::viewportRectChanged,viewport,&Viewport::onRectChanged);
    connect(this,&QMCanvasScene::viewportPixmapChanged,viewport,&Viewport::onPixmapChanged);

    connect(view,&View::scaleFactorChanged,this,&QMCanvasScene::onScaleBy);
    connect(view->horizontalScrollBar(),&QScrollBar::valueChanged,this,&QMCanvasScene::onHScrollBarChanged);
    connect(view->verticalScrollBar(),&QScrollBar::valueChanged,this,&QMCanvasScene::onVScrollBarChanged);
    connect(this,&QMCanvasScene::viewPropertyChanged,view,&View::onPropertyChanged);

    connect(canvasView,&QMCanvasView::canvasSceneChanged,viewport,&Viewport::onSceneChanged);
    connect(canvasView,&QMCanvasView::sizeChanged,this,&QMCanvasScene::onSizeChanged);
}

qreal QMCanvasScene::factor() const{
    return location_.factor();
}

void QMCanvasScene::setFactor(qreal factor){
    location_.setFactor(factor);
}

qreal QMCanvasScene::extraViewportMargin() const{
    return location_.extraViewportMargin();
}

void QMCanvasScene::setExtraViewportMargin(qreal rate){
    location_.setExtraViewportMargin(rate);

    inform();
}

qreal QMCanvasScene::ratio() const{
    return location_.ratio();
}

void QMCanvasScene::setRatio(qreal ratio){
    location_.setRatio(ratio);

    inform();
}

QMDrawObject* QMCanvasScene::activeDrawObject(){
    return layerManager_.activeObject()->activeObject();
}

void QMCanvasScene::setActiveDrawObject(QMDrawObject* object){
    layerManager_.activeObject()->setActiveObject(QSharedPointer<QMDrawObject>(object));
}

void QMCanvasScene::switchDrawObject(int i){
    layerManager_.activeObject()->switchActiveObject(i);
}

void QMCanvasScene::finishActiveDrawObject(){
    layerManager_.activeObject()->finishActiveObject();
}

Layer* QMCanvasScene::activeLayer(){
    return layerManager_.activeObject();
}

void QMCanvasScene::setActiveLayer(Layer* layer){
    layerManager_.setActiveObject(QSharedPointer<Layer>(layer));
}

void QMCanvasScene::switchLayer(int i){
    layerManager_.switchActiveObject(i);
}

void QMCanvasScene::finishActiveLayer(){
    layerManager_.finishActiveObject();
}

const LayerManager& QMCanvasScene::layerManager(){
    return layerManager_;
}

void QMCanvasScene::swapLayer(int a, int b){
    layerManager_.swap(a,b);
}

bool QMCanvasScene::isLayerHide(int index){
    return layerManager_.hide(index);
}

void QMCanvasScene::setLayerHide(int index, bool hide){
    layerManager_.setHide(index,hide);
}

void QMCanvasScene::inform(){
    qreal x = location_.viewportRectRM().x();
    qreal y = location_.viewportRectRM().y();
    QSizeF size = location_.viewRectR().size();
    emit viewPropertyChanged(QPoint(x,y),size.toSize());
    emit viewportRectChanged();
    emit viewportPixmapChanged();
}

void QMCanvasScene::onScaleBy(bool magnify, QPoint point){
    location_.setMousePoint(QPoint(point.x()/ratio(),point.y()/ratio()));
    if (magnify){
        qreal r = ratio()*factor();
        if (r>maxRatio()) r = maxRatio();
        setRatio(r);
    }else{
        qreal r = ratio()/factor();
        if (r<minRatio()) r = minRatio();
        setRatio(r);
    }
}

void QMCanvasScene::onSizeChanged(){
    inform();
}

void QMCanvasScene::onMouseMove(QPoint point){
    if (activeDrawObject()==nullptr) return;
    int x = point.x() / location_.ratio();
    int y = point.y() / location_.ratio();
    activeDrawObject()->recordPoint(QPoint(x,y));
    inform();
}

void QMCanvasScene::onMousePress(QPoint point){
    if (activeDrawObject()==nullptr) return;
    int x = point.x() / location_.ratio();
    int y = point.y() / location_.ratio();
    activeDrawObject()->begin(QPoint(x,y));
    inform();
}

void QMCanvasScene::onMouseRelease(QPoint point){
    if (activeDrawObject()==nullptr) return;
    int x = point.x() / location_.ratio();
    int y = point.y() / location_.ratio();
    activeDrawObject()->end(QPoint(x,y));
    finishActiveDrawObject();
    inform();
}

void QMCanvasScene::onHScrollBarChanged(int value){
    //在原有的rect上改会出bug，疑似是因为隐式共享
    QRectF vr = location_.viewportRectRM();
    QRectF rect = QRectF(value,vr.y(),vr.width(),vr.height());
    location_.setViewportRect(rect);

    inform();
}

void QMCanvasScene::onVScrollBarChanged(int value){
    QRectF vr = location_.viewportRectRM();
    QRectF rect = QRectF(vr.x(),value,vr.width(),vr.height());
    location_.setViewportRect(rect);

    inform();
}

void QMCanvasScene::onCtrlAndZ(){
    layerManager_.activeObject()->undo();
}

void QMCanvasScene::onCtrlAndY(){
    layerManager_.activeObject()->redo();
}
