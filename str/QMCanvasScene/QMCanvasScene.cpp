// Copyright (c) 2026 Ximiaw
// SPDX-License-Identifier: MIT

#include "QMCanvasScene.h"
#include "QMCanvasView.h"
#include "Viewport.h"
#include "View.h"

QMCanvasScene::QMCanvasScene(QPixmap pixmap,QObject* parent)
    :QObject(parent){
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
    return maxRatio_;
}

void QMCanvasScene::setMaxRatio(qreal max){
    maxRatio_ = max;
}

qreal QMCanvasScene::minRatio() const{
    return minRatio_;
}

void QMCanvasScene::setMinRatio(qreal min){
    minRatio_ = min;
}

void QMCanvasScene::setPixmap(QPixmap& pixmap){
    viewportRect_ = QRect(pixmap.rect());
    pixmap_ = pixmap;
    setRatio(1.0);//这个函数会触发更新
}

const QPixmap& QMCanvasScene::pixmap() const{
    return pixmap_;
}

QPixmap QMCanvasScene::getViewportPixmap(){
    qreal x = viewportRect_.x();
    qreal y = viewportRect_.y();
    qreal w = viewportRect_.width() * extraViewportMargin();
    qreal h = viewportRect_.height() * extraViewportMargin();

    x = x - (w - viewportRect_.width()) / 2;
    y = y - (h - viewportRect_.height()) / 2;

    QRectF rect(x,y,w,h);

    QPixmap viewportPixmap = pixmap().copy(rect.toRect())
        .scaled(w*ratio(),h*ratio(),Qt::KeepAspectRatio, Qt::FastTransformation);
    return viewportPixmap;
}

QRect QMCanvasScene::getViewportRect(){
    qreal x = viewportRect_.x() * ratio();
    qreal y = viewportRect_.y() * ratio();
    qreal w = viewportRect_.width() * ratio() * extraViewportMargin();
    qreal h = viewportRect_.height() * ratio() * extraViewportMargin();

    qreal dw = (w - viewportRect_.width() * ratio()) / 2;
    qreal dh = (h - viewportRect_.height() * ratio()) / 2;

    x = x - dw;
    y = y - dh;

    QRectF rect(x,y,w,h);

    if (mousePoint_.x() >= 0 || mousePoint_.y() >= 0){
        qreal dw_m = mousePoint_.x() - viewportRect_.x();
        qreal dh_m = mousePoint_.y() - viewportRect_.y();

        qreal dw_m_r = mousePoint_.x() * ratio();
        qreal dh_m_r = mousePoint_.y() * ratio();
        mousePoint_ = QPoint(-1,-1);

        rect = QRect(
            dw_m_r - dw_m,
            dh_m_r - dh_m,
            rect.width(),
            rect.height()
        );
    }

    qreal qw = pixmap().width() * ratio();
    qreal qh = pixmap().height() * ratio();

    if (qw < rect.x() + rect.width()){
        rect.setX(qw - w);
    }else if (rect.x() < 0){
        rect.setX(0);
    }
    if (qh < rect.y() + rect.height()){
        rect.setY(qh - h);
    }else if (rect.y() < 0){
        rect.setY(0);
    }
    return rect.toRect();
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
    return factor_;
}

void QMCanvasScene::setFactor(qreal factor){
    factor_ = factor;
}

qreal QMCanvasScene::extraViewportMargin() const{
    return marginRate_;
}

void QMCanvasScene::setExtraViewportMargin(qreal rate){
    marginRate_ = rate;
    inform();
}

qreal QMCanvasScene::ratio() const{
    return ratio_;
}

void QMCanvasScene::setRatio(qreal ratio){
    ratio_=ratio;
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

    qreal x = viewportRect_.x() * ratio();
    qreal y = viewportRect_.y() * ratio();
    qreal pix_w = pixmap_.width() * ratio();
    qreal pix_h = pixmap_.height() * ratio();
    emit viewPropertyChanged(QPoint(x,y),QSize(pix_w,pix_h));
}

void QMCanvasScene::onViewportChanged(QRectF rect){
    viewportRect_=QRect(rect.x()/ratio(),rect.y()/ratio()
        ,rect.width()/ratio(),rect.height()/ratio());
    inform();
}

void QMCanvasScene::onScaleBy(bool magnify, QPoint point){
    mousePoint_ = QPoint(point.x()/ratio(),point.y()/ratio());
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
    viewportRect_ = QRectF(value/ratio(),viewportRect_.y(),viewportRect_.width(),viewportRect_.height());
    inform();
}

void QMCanvasScene::onVScrollBarChanged(int value){
    viewportRect_ = QRectF(viewportRect_.x(),value/ratio(),viewportRect_.width(),viewportRect_.height());
    inform();
}

void QMCanvasScene::onCtrlAndZ(){
    if (drawObject_.isEmpty()) return;
    deleteGraphic(graphicList().back());
}

void QMCanvasScene::onCtrlAndY(){
    //重做
}
