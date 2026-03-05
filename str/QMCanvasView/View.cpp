//
// Created by m3311 on 2026/2/28.
//

#include "View.h"

View::View(QWidget *parent):QScrollArea(parent) {
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
            translation();
            QScrollArea::wheelEvent(event);
            break;
        case WheelMode::ZOOM:
            renderScaled(event);
            break;
    }
}

void View::renderScaled(QWheelEvent *event) {
    //
}
