//
// Created by m3311 on 2026/2/28.
//

#include "QMCanvasView.h"

QMCanvasView::QMCanvasView() {

}

QMCanvasView::~QMCanvasView() {

}

QWidget* QMCanvasView::viewport() const {
    return  QScrollArea::viewport();
}

void QMCanvasView::setWheelMode(WheelMode wheel_mode) {
    wheel_mode_=wheel_mode;
}

WheelMode QMCanvasView::wheelMode() const {
    return wheel_mode_;
}

void QMCanvasView::wheelEvent(QWheelEvent *event) {
    switch (wheelMode()) {
        case WheelMode::ROLL:
            QScrollArea::wheelEvent(event);
            break;
        case WheelMode::ZOOM:
            renderScaled(event);
            break;
    }
}