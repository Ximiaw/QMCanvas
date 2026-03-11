//
// Created by m3311 on 2026/2/28.
//

#include "View.h"

View::View(QWidget *parent):QScrollArea(parent) {
    setWidget(new QWidget);
}

View::~View() = default;

void View::setWheelMode(WheelMode mode) {
    if (wheelMode_!=mode)
        wheelMode_=mode;
}
WheelMode View::wheelMode() const {
    return wheelMode_;
}

void View::wheelEvent(QWheelEvent *event) {
    switch (wheelMode()) {
        case WheelMode::ROLL:
            QScrollArea::wheelEvent(event);
            translation();
            break;
        case WheelMode::ZOOM:
            factor(event);
            break;
    }
}

void View::translation(){
    QRect rect(
        horizontalScrollBar()->value()
        ,verticalScrollBar()->value()
        ,viewport()->width()
        ,viewport()->height());
    emit viewportChanged(rect);
}

void View::factor(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    QPoint numSteps = numDegrees / 8 / 15;
    if (numSteps.y()>=1 || numSteps.y()<=-1)
        emit scaleFactorChanged(numSteps.y()>0,widget()->mapFromGlobal(QCursor::pos()));
}

void View::onPropertyChanged(QPoint point,QSize size){
    widget()->resize(size);
    horizontalScrollBar()->setValue(point.x());
    verticalScrollBar()->setValue(point.y());
}