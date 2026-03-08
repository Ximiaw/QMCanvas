//
// Created by m3311 on 2026/3/1.
//

#include "QMCanvasScene.h"

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

void QMCanvasScene::setPixmap(QPixmap& pixmap){
    viewportRect_=pixmap.rect();
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

    QPixmap viewportPixmap = pixmap().copy(rect.toRect()).scaled(w*ratio(),h*ratio());
    return viewportPixmap;
}

QRect QMCanvasScene::getViewportRect(){
    qreal x = viewportRect_.x() * ratio();
    qreal y = viewportRect_.y() * ratio();
    qreal w = viewportRect_.width() * ratio() * extraViewportMargin();
    qreal h = viewportRect_.height() * ratio() * extraViewportMargin();

    x = x - (w - viewportRect_.width()) / 2;
    y = y - (h - viewportRect_.height()) / 2;

    QRectF rect(x,y,w,h);

    qreal qw = pixmap().width() * ratio();
    qreal qh = pixmap().height() * ratio();

    if (w > pixmap_.width()) w = pixmap_.width();
    else if (w < 0) w = 0;
    if (h > pixmap_.height()) h = pixmap_.height();
    else if (h < 0) h = 0;
    if (qw < x + w){
        rect.setX(qw - w);
    }else if (x + w < 0){
        rect.setX(0);
    }
    if (qh < y + h){
        rect.setY(qh - h);
    }else if (y + h < 0){
        rect.setY(0);
    }
    return rect.toRect();
}

void QMCanvasScene::updatePixmap(QPainter* painter){
    painter->drawPixmap(getViewportRect(),getViewportPixmap());
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
    if (activeDrawObject_){
        activeDrawObject_->setParent(nullptr);
        activeDrawObject_->deleteLater();
    }
    activeDrawObject_=object;
    object->setParent(this);
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
    viewportRect_=rect;
    inform();
}

void QMCanvasScene::onScaleBy(bool magnify, QPoint point){
    if (magnify){
        setRatio(ratio()*factor());
    }else{
        setRatio(ratio()/factor());
    }
    /*
     *
     *
     *后续再处理光标的偏移
     *注意传过来的光标位置需要转换为图片没有缩放的情况下的位置
     *
     *
    */
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
    addGraphic(activeDrawObject());
    activeDrawObject_=nullptr;
}