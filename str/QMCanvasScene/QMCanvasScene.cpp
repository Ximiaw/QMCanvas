//
// Created by m3311 on 2026/3/1.
//

#include "QMCanvasScene.h"

QMCanvasScene::QMCanvasScene(QObject* parent)
    :QObject(parent){}

QMCanvasScene::QMCanvasScene(QPixmap pixmap,QObject* parent)
    :QMCanvasScene(parent){
    setPixmap(pixmap);
    viewportRect_=pixmap.rect();
}

const QList<QMDrawObject*> QMCanvasScene::graphicList() const{
    return drawObject_;
}

void QMCanvasScene::addGraphic(QMDrawObject* graphic){
    graphic->setParent(this);
    drawObject_.append(graphic);
}

QMDrawObject* QMCanvasScene::removeGraphic(QMDrawObject* graphic){
    if (!graphic || drawObject_.indexOf(graphic) == -1) return nullptr;
    graphic->setParent(nullptr);
    drawObject_.removeOne(graphic);
    return graphic;
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
    emit viewportPixmapChanged();
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
    if (qw < x + w){
        rect.setX(qw - w);
    }
    if (qh < y + h){
        rect.setY(qh - h);
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
    emit viewportRectChanged();
    emit viewportPixmapChanged();
}

qreal QMCanvasScene::ratio() const{
    return ratio_;
}

void QMCanvasScene::setRatio(qreal ratio){
    ratio_=ratio;
    emit viewportRectChanged();
    emit viewportPixmapChanged();
}

QMDrawObject* QMCanvasScene::activeDrawObject() const{
    return activeDrawObject_;
}

void QMCanvasScene::setActiveDrawObject(QMDrawObject* object){
    if (!activeDrawObject_){
        activeDrawObject_->setParent(nullptr);
        activeDrawObject_->deleteLater();
    }
    activeDrawObject_=object;
    object->setParent(this);
}

void QMCanvasScene::onViewportChanged(QRectF rect){
    viewportRect_=rect;
    emit viewportRectChanged();
    emit viewportPixmapChanged();
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
    emit viewportRectChanged();
    emit viewportPixmapChanged();
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
