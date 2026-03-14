// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMCanvasScene.h"
#include "QMCanvasView.h"
#include "Viewport.h"
#include "View.h"

QMCanvasScene::QMCanvasScene(QPixmap pixmap,QObject* parent)
    :QObject(parent),location_(pixmap.rect()){
    setPixmap(pixmap);
}

const QList<QMDrawObject*> QMCanvasScene::graphicList() const{
    return drawObject_;
}

void QMCanvasScene::addGraphic(QMDrawObject* graphic){
    graphic->setParent(this);
    drawObject_.append(graphic);
}

bool QMCanvasScene::deleteGraphic(QMDrawObject* graphic){
    if (!graphic || drawObject_.indexOf(graphic) == -1) return false;
    graphic->setParent(nullptr);
    drawObject_.removeOne(graphic);
    graphic->deleteLater();
    return true;
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

void QMCanvasScene::setPixmap(QPixmap& pixmap){
    //viewportRect_ = QRect(pixmap.rect());
    pixmap_ = pixmap;
    setRatio(1.0);//这个函数会触发更新
}

const QPixmap& QMCanvasScene::pixmap() const{
    return pixmap_;
}

QPixmap QMCanvasScene::getViewportPixmap(){
    QRectF rect(location_.viewportRect());
    QSize size = location_.viewportRectRM().size().toSize();
    QPixmap viewportPixmap = pixmap().copy(rect.toRect())
        .scaled(size,Qt::KeepAspectRatio, Qt::FastTransformation);
    return viewportPixmap;
}

QRect QMCanvasScene::getViewportRect(){
    return location_.viewportRectRM().toRect();
}

void QMCanvasScene::updatePixmap(QPainter* painter){
    QPixmap pixmap =getViewportPixmap();
    QRect rect = pixmap.rect();
    painter->drawPixmap(rect,pixmap);
}

void QMCanvasScene::draw(QPainter* painter){
    // if (drawObject_.count()>=20){
    //
    // //开启子线程合并到图片上，或者子线程持续开启，检测是否大于30，如果大于30这里给出合并的许可，将最老的10次操作合并到pixmap的基底上
    //
    // }

    for (auto i=drawObject_.constBegin();i!=drawObject_.constEnd();i++){
        (*i)->draw(painter);
    }
}

void QMCanvasScene::init(QMCanvasView* canvasView,View* view,Viewport* viewport){
    view->widget()->setGeometry(pixmap_.rect());

    connect(viewport,&Viewport::mouseMove,this,&QMCanvasScene::onMouseMove);
    connect(viewport,&Viewport::mouseRelease,this,&QMCanvasScene::onMouseRelease);
    connect(viewport,&Viewport::mousePress,this,&QMCanvasScene::onMousePress);
    connect(viewport,&Viewport::ctrlAndZ,this,&QMCanvasScene::onCtrlAndZ);
    connect(viewport,&Viewport::ctrlAndY,this,&QMCanvasScene::onCtrlAndY);
    connect(this,&QMCanvasScene::viewportRectChanged,viewport,&Viewport::onRectChanged);
    connect(this,&QMCanvasScene::viewportPixmapChanged,viewport,&Viewport::onPixmapChanged);

    connect(view,&View::viewportChanged,this,&QMCanvasScene::onViewportChanged);
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

QMDrawObject* QMCanvasScene::activeDrawObject() const{
    return activeDrawObject_;
}

void QMCanvasScene::setActiveDrawObject(QMDrawObject* object){
    if (!object) return;
    if (drawObject_.contains(object)) return;
    if (activeDrawObject_){
        activeDrawObject_->setParent(nullptr);
        activeDrawObject_->deleteLater();
    }
    activeDrawObject_=object;
    object->setParent(this);
}

void QMCanvasScene::finishActiveDrawObject(){
    if (activeDrawObject_){
        addGraphic(activeDrawObject_);
        activeDrawObject_=nullptr;
    }
}

void QMCanvasScene::inform(){
    emit viewportRectChanged();
    emit viewportPixmapChanged();

    qreal x = location_.viewportRectRM().x();
    qreal y = location_.viewportRectRM().y();
    qreal pix_w = location_.baseRect().width() * ratio();
    qreal pix_h = location_.baseRect().height() * ratio();
    emit viewPropertyChanged(QPoint(x,y),QSize(pix_w,pix_h));
}

void QMCanvasScene::onViewportChanged(QRectF rect){
    qreal x = rect.x()/location_.ratio();
    qreal y = rect.y()/location_.ratio();
    qreal w = rect.width()/location_.ratio();
    qreal h = rect.height()/location_.ratio();
    location_.setViewportRect(QRectF(x,y,w,h));

    inform();
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
    activeDrawObject()->recordPoint(point);
}

void QMCanvasScene::onMousePress(QPoint point){
    if (activeDrawObject()==nullptr) return;
    activeDrawObject()->begin(point);
}

void QMCanvasScene::onMouseRelease(QPoint point){
    if (activeDrawObject()==nullptr) return;
    activeDrawObject()->end(point);
    finishActiveDrawObject();
}

void QMCanvasScene::onHScrollBarChanged(int value){
    //在原有的rect上改会出bug，疑似是因为隐式共享
    QRectF vr = location_.viewportRect();
    QRectF rect = QRectF(value/ratio(),vr.y(),vr.width(),vr.height());
    location_.setViewportRect(rect);

    inform();
}

void QMCanvasScene::onVScrollBarChanged(int value){
    QRectF vr = location_.viewportRect();
    QRectF rect = QRectF(vr.x(),value/ratio(),vr.width(),vr.height());
    location_.setViewportRect(rect);

    inform();
}

void QMCanvasScene::onCtrlAndZ(){
    if (drawObject_.isEmpty()) return;
    deleteGraphic(graphicList().back());
}

void QMCanvasScene::onCtrlAndY(){
    //重做
}
